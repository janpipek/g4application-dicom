#include "DicomGeometryBuilder.hh"

#include <G4LogicalVolume.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>

#include "DicomMaterialDatabase.hh"
#include "DicomData.hh"

using namespace g4dicom;
using namespace std;

// See http://nipy.org/nibabel/dicom/dicom_orientation.html

void DicomGeometryBuilder::BuildGeometry(G4LogicalVolume* logWorld)
{
    if (!_data)
    {
        // Throw an exception
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
    new G4PVPlacement(0,                // Rotation
        G4ThreeVector(0.0, 0.0, 100.0), // Position
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
        new G4Box(containerName, voxelSize[0], voxelSize[1], voxelSize[2]);
    G4LogicalVolume* logContainer =
        new G4LogicalVolume(solContainer, air, containerName);

    G4String xReplicaName("voxelX");
    G4String yReplicaName("voxelY");
    G4String zReplicaName("voxelZ");

    // Solids
    G4VSolid* solXReplica =
        new G4Box(xReplicaName, voxelSize[0], voxelSize[1], voxelSize[2]);
    G4VSolid* solYReplica =
        new G4Box(yReplicaName, totalSize[0], voxelSize[1], voxelSize[2]);
    G4VSolid* solZReplica =
        new G4Box(zReplicaName, totalSize[0], totalSize[1], voxelSize[2]);

    // Logical volumes
    G4LogicalVolume* logXReplica =
        new G4LogicalVolume(solXReplica, air, xReplicaName);
    G4LogicalVolume* logYReplica =
        new G4LogicalVolume(solYReplica, air, yReplicaName);
    G4LogicalVolume* logZReplica =
        new G4LogicalVolume(solZReplica, air, zReplicaName);

    // Physical volumes
    return logContainer;
}

DicomGeometryBuilder::DicomGeometryBuilder()
    : _data(0), _materialDatabase(0)
{

}

DicomGeometryBuilder::~DicomGeometryBuilder()
{
    // _data is deleted by the originating DicomReader.
}
