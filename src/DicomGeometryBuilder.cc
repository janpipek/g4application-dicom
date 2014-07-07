#include "DicomGeometryBuilder.hh"

#include <stdexcept>

#include <G4LogicalVolume.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4PVParameterised.hh>
#include <G4VisAttributes.hh>
#include <globals.hh>

#include "Configuration.hh"

#include "VMaterialDatabase.hh"
#include "DicomData.hh"
#include "dicomConfiguration.hh"
#include "VoxelParameterisation.hh"

using namespace g4dicom;
using namespace std;
using namespace g4;

// See http://nipy.org/nibabel/dicom/dicom_orientation.html

void DicomGeometryBuilder::ConfigurationChanged(const std::string& key)
{
    if (key == VIS_SHOW_VOXELS)
    {
        bool val = Configuration::GetValue<int>(key);
        SetVoxelsVisible(val);
    }
    else if (key == PHANTOM_CENTER_X)
    {
        double val = Configuration::GetValue<double>(key) * mm;
        G4ThreeVector newCenter(val, _phantomCenter.getY(), _phantomCenter.getZ());
        SetPhantomCenter(newCenter);
    }
    else if (key == PHANTOM_CENTER_Y)
    {
        double val = Configuration::GetValue<double>(key) * mm;
        G4ThreeVector newCenter(_phantomCenter.getX(), val, _phantomCenter.getZ());
        SetPhantomCenter(newCenter);
    }
    else if (key == PHANTOM_CENTER_Z)
    {
        double val = Configuration::GetValue<double>(key) * mm;
        G4ThreeVector newCenter(_phantomCenter.getX(), _phantomCenter.getY(), val);
        SetPhantomCenter(newCenter);
    }
}

void DicomGeometryBuilder::SetVoxelsVisible(bool value)
{
    _voxelsVisible = value;
    Configuration::SetValue(VIS_SHOW_VOXELS, value);
}

void DicomGeometryBuilder::SetPhantomCenter(const G4ThreeVector& position)
{
    bool positionChanged = (_phantomCenter != position);
    if (positionChanged)
    {
        _phantomCenter = position;
        Configuration::SetValue(PHANTOM_CENTER_X, position.getX() / mm);
        Configuration::SetValue(PHANTOM_CENTER_Y, position.getX() / mm);
        Configuration::SetValue(PHANTOM_CENTER_Z, position.getX() / mm);

        if (_physContainer)
        {
            _physContainer->SetTranslation(_phantomCenter); // Is this ok?
            GeometryChanged();
        }
    }
}

void DicomGeometryBuilder::BuildGeometry(G4LogicalVolume* logWorld)
{
    if (!_data)
    {
        throw runtime_error("Cannot build geometry without DICOM data.");
    }
    if (!_materialDatabase)
    {
        throw runtime_error("Cannot build geometry without material data.");   
    }
    vector<int> dims = _data->GetDimensions();
    G4cout << "Building DICOM voxel geometry of "
        << dims[0] << " x " << dims[1] << " x " << dims[2]
        << "..." << G4endl;

    vector<double> totalSize = _data->GetTotalSize();
    G4cout << "Total size is " << totalSize[0]
        << " x " << totalSize[1] << " x " << totalSize[2] << " mm."
        << G4endl;

    G4LogicalVolume* logContainer = BuildLogicalVolume();
    _physContainer = 
        new G4PVPlacement(0,                // Rotation
            _phantomCenter,                 // Position
            logContainer,                   // The logic volume
            "phantomContainer",             // Name
            logWorld,                       // Mother
            false,                          // No op. bool.
            1);                             // Copy number
}

G4LogicalVolume *DicomGeometryBuilder::BuildLogicalVolume()
{
    vector<double> voxelSize = _data->GetVoxelSize();
    vector<double> totalSize = _data->GetTotalSize();
    vector<int> dims = _data->GetDimensions();

    // Fill everything with air
    G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    // The container
    G4String containerName("voxelContainer");
    G4VSolid* solContainer =
        new G4Box(containerName, totalSize[0]/2, totalSize[1]/2, totalSize[2]/2);
    G4LogicalVolume* logContainer =
        new G4LogicalVolume(solContainer, air, containerName);

    G4String xReplicaName("voxelX");
    G4String yReplicaName("voxelY");
    G4String zReplicaName("voxelZ");

    // Solids
    G4VSolid* solXReplica =
        new G4Box(xReplicaName, voxelSize[0]/2, totalSize[1]/2, totalSize[2]/2);
    G4VSolid* solYReplica =
        new G4Box(yReplicaName, voxelSize[0]/2, voxelSize[1]/2, totalSize[2]/2);
    G4VSolid* solZVoxel =
        new G4Box(zReplicaName, voxelSize[0]/2, voxelSize[1]/2, voxelSize[2]/2);

    // Logical volumes
    G4LogicalVolume* logXReplica =
        new G4LogicalVolume(solXReplica, air, xReplicaName);
    G4LogicalVolume* logYReplica =
        new G4LogicalVolume(solYReplica, air, yReplicaName);
    G4LogicalVolume* logZVoxel =
        new G4LogicalVolume(solZVoxel, air, zReplicaName);

    logXReplica->SetVisAttributes(G4VisAttributes::Invisible);
    logYReplica->SetVisAttributes(G4VisAttributes::Invisible); 
    if (!_voxelsVisible)
    {
        logZVoxel->SetVisAttributes(G4VisAttributes::Invisible);
    }

    // Place physical volumes for x&y
    new G4PVReplica(xReplicaName, logXReplica, logContainer, kXAxis, dims[0], voxelSize[0]);
    new G4PVReplica(yReplicaName, logYReplica, logXReplica, kYAxis, dims[1], voxelSize[1]);

    // Place physical volume with parameterisation for z
    VoxelParameterisation* voxelParam = new VoxelParameterisation(_materialDatabase, _data);
    new G4PVParameterised(zReplicaName,
                          logZVoxel,
                          logYReplica,
                          kZAxis,
                          dims[2],
                          voxelParam);

    // Physical volumes
    return logContainer;
}

DicomGeometryBuilder::DicomGeometryBuilder()
    : _data(0), _materialDatabase(0), _physContainer(0)
{
    _phantomCenter.setX(Configuration::GetValue(PHANTOM_CENTER_X, 0.));
    _phantomCenter.setY(Configuration::GetValue(PHANTOM_CENTER_Y, 0.));
    _phantomCenter.setZ(Configuration::GetValue(PHANTOM_CENTER_Z, 0.));
}

DicomGeometryBuilder::~DicomGeometryBuilder()
{
    // _data is deleted by the originating DicomReader.
}
