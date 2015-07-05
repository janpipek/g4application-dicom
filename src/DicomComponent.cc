#include "DicomComponent.hh"

#include <G4Box.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4PVParameterised.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>
#include <G4RunManager.hh>
#include <globals.hh>

#include "dicomConfiguration.hh"
#include "DicomReader.hh"
#include "DicomData.hh"
#include "MaterialDatabase.hh"
#include "MaterialJsonReader.hh"
#include "DicomMessenger.hh"
#include "VoxelParameterisation.hh"

using namespace g4;
using namespace g4dicom;
using namespace std;

// See http://nipy.org/nibabel/dicom/dicom_orientation.html

DicomComponent::DicomComponent()
    : _data(nullptr), _materialDatabase(nullptr), _autoCrop(false)
{
    SetConfigurationDefaults();

    _reader = new DicomReader();
    _messenger = new DicomMessenger(*this);
}

void DicomComponent::ConfigurationChanged(const string &key)
{
    if (key == VIS_SHOW_VOXELS)
    {
        bool val = Configuration::Get<int>(key);
        SetVoxelsVisible(val);
    }
    else if (key == PHANTOM_CENTER_X)
    {
        double val = Configuration::Get<double>(key) * mm;
        G4ThreeVector newCenter(val, _phantomCenter.getY(), _phantomCenter.getZ());
        SetPhantomCenter(newCenter);
    }
    else if (key == PHANTOM_CENTER_Y)
    {
        double val = Configuration::Get<double>(key) * mm;
        G4ThreeVector newCenter(_phantomCenter.getX(), val, _phantomCenter.getZ());
        SetPhantomCenter(newCenter);
    }
    else if (key == PHANTOM_CENTER_Z)
    {
        double val = Configuration::Get<double>(key) * mm;
        G4ThreeVector newCenter(_phantomCenter.getX(), _phantomCenter.getY(), val);
        SetPhantomCenter(newCenter);
    }
    else if (key == PHANTOM_ROTATION_PHI)
    {
        double val = Configuration::Get<double>(key) * deg;
        G4RotationMatrix rotation = GetPhantomRotation();
        rotation.setPhi(val);
        SetPhantomRotation(rotation);
    }
    else if (key == PHANTOM_ROTATION_THETA)
    {
        double val = Configuration::Get<double>(key) * deg;
        G4RotationMatrix rotation = GetPhantomRotation();
        rotation.setTheta(val);
        SetPhantomRotation(rotation);
    }
    else if (key == PHANTOM_ROTATION_PSI)
    {
        double val = Configuration::Get<double>(key) * deg;
        G4RotationMatrix rotation = GetPhantomRotation();
        rotation.setPsi(val);
        SetPhantomRotation(rotation);
    }
}

void DicomComponent::BuildGeometry(G4LogicalVolume* logVolume)
{
    if (!_data)
    {
        LoadDicomData();
    }
    if (!_data)
    {
        // TODO: throw
    }
    if (_autoCrop)
    {
        _data->AutoCrop(_autoCropMinHU);
    }
    else if (_cropLimits)
    {
        _data->Crop(*_cropLimits);
    }

    if (!_data->IsValid())
    {
        G4Exception("DicomPlugin",
            "InvalidDicomData", FatalException,
            "Cannot interpret DICOM slices as a single voxel array."
        );
    }


    // TODO: Change to automatic reading? A bit cumbersome now
    if (!_materialDatabase)
    {
        // TODO: throw
    }

    vector<int> dims = _data->GetDimensions();
    G4cout << "Building DICOM voxel geometry of "
        << dims[0] << " x " << dims[1] << " x " << dims[2]
        << "..." << G4endl;

    vector<double> totalSize = _data->GetTotalSize();
    G4cout << "Total size is " << totalSize[0]
        << " x " << totalSize[1] << " x " << totalSize[2] << " mm."
        << G4endl;

    G4LogicalVolume* logContainer = BuildLogicalVolume(_data);

    new G4PVPlacement(&_rotationMatrix, // Rotation
        _phantomCenter,                 // Position
        logContainer,                   // The logic volume
        "phantomContainer",             // Name
        logVolume,                      // Mother
        false,                          // No op. bool.
        1);                             // Copy number
}

void DicomComponent::LoadDicomData()
{
    _data = _reader->GetData();
}

G4LogicalVolume* DicomComponent::BuildLogicalVolume(DicomData* data)
{
    vector<double> voxelSize = data->GetVoxelSize();
    vector<double> totalSize = data->GetTotalSize();
    vector<int> dims = data->GetDimensions();

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
    VoxelParameterisation* voxelParam = new VoxelParameterisation(_materialDatabase, data);
    new G4PVParameterised(zReplicaName,
                          logZVoxel,
                          logYReplica,
                          kZAxis,
                          dims[2],
                          voxelParam);

    // Physical volumes
    return logContainer;
}

void DicomComponent::SetMaterialDatabase(VMaterialDatabase *database)
{
    if (_materialDatabase)
    {
        delete _materialDatabase;
    }
    _materialDatabase = database;
}

void DicomComponent::LoadMaterialDatabase(const string& path)
{
    MaterialDatabase* db  = new MaterialDatabase();
    MaterialJsonReader reader;
    auto templates = reader.LoadTemplates(path);
    for (auto it = templates.begin(); it != templates.end(); it++)
    {
        db->AddMaterialTemplate(*it);
    }
    SetMaterialDatabase(db);
}

void DicomComponent::SetCropLimits(const vector<int>& cropLimits)
{
    _cropLimits = new vector<int>();
    *_cropLimits = cropLimits;
}

void DicomComponent::SetAutoCrop(double minHU)
{
    _autoCrop = true;
    _autoCropMinHU = minHU;
}

void DicomComponent::SetVoxelsVisible(bool value)
{
    _voxelsVisible = value;
    Configuration::Set(VIS_SHOW_VOXELS, value);
}

void DicomComponent::SetPhantomCenter(const G4ThreeVector& position)
{
    bool positionChanged = (_phantomCenter != position);
    if (positionChanged)
    {
        _phantomCenter = position;
        Configuration::Set(PHANTOM_CENTER_X, position.getX() / mm);
        Configuration::Set(PHANTOM_CENTER_Y, position.getY() / mm);
        Configuration::Set(PHANTOM_CENTER_Z, position.getZ() / mm);

        if (_physContainer)
        {
            _physContainer->SetTranslation(_phantomCenter);
            G4RunManager::GetRunManager()->GeometryHasBeenModified();
        }
    }
}

void DicomComponent::SetPhantomRotation(const G4RotationMatrix& rotation)
{
    bool rotationChanged = (_rotationMatrix != rotation);
    if (rotationChanged)
    {
        _rotationMatrix = rotation;
        Configuration::Set(PHANTOM_ROTATION_PHI, rotation.getPhi() / deg, this);
        Configuration::Set(PHANTOM_ROTATION_THETA, rotation.getTheta() / deg, this);
        Configuration::Set(PHANTOM_ROTATION_PSI, rotation.getPsi() / deg, this);

        if (_physContainer)
        {
            _physContainer->SetRotation(&_rotationMatrix);
            G4RunManager::GetRunManager()->GeometryHasBeenModified();
        }
    }
}
