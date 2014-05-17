#include "VoxelParameterisation.hh"

#include <G4VTouchable.hh>
#include <G4VPhysicalVolume.hh>

#include "VMaterialDatabase.hh"
#include "DicomData.hh"

using namespace g4dicom;
using namespace boost;
using namespace std;

G4Material* VoxelParameterisation::ComputeMaterial(G4VPhysicalVolume *currentVol, const G4int repNo, const G4VTouchable *parentTouch)
{
    if (!parentTouch)
    {
        return _materialDatabase->GetDefaultMaterial();
    }
    int x = parentTouch->GetReplicaNumber(1);
    int y = parentTouch->GetReplicaNumber(0);
    int z = repNo;
    if (z < 0) // Deal with occasional negative replica number (where from?)
    {
        G4cerr << "VoxelParameterisation: z=" << z << " requested. Continuing..." << G4endl;
        z = 0;
    }
    return _voxelMaterials[x][y][z];
}

void VoxelParameterisation::ComputeTransformation(const G4int copyNo,
        G4VPhysicalVolume* physVol) const
{
    // Adapted from DicomNestedPhantomParameterisation (in the DICOM example)
    G4ThreeVector position(0., 0., (2 * copyNo + 1 - _dims[2]) * _zHalfSize);
    physVol->SetTranslation(position);
}

G4int VoxelParameterisation::GetNumberOfMaterials() const
{
    return _materials.size();
}

G4Material* VoxelParameterisation::GetMaterial(G4int idx) const
{
    return _materials[idx];
}

VoxelParameterisation::VoxelParameterisation(VMaterialDatabase* materialDatabase, DicomData* dicomData)
    : _materialDatabase(materialDatabase),
      _dicomData(dicomData)
{
    // Prepare storage for materials
    _dims = dicomData->GetDimensions();
    _zHalfSize = dicomData->GetVoxelSize()[2] / 2.0;
    _voxelMaterials.resize(boost::extents[_dims[0]][_dims[1]][_dims[2]]);

    // Fill in materials
    for (int x = 0; x < _dims[0]; x++)
    {
        for (int y = 0; y < _dims[1]; y++)
        {
            for (int z = 0; z < _dims[2]; z++)
            {
                _voxelMaterials[x][y][z] =
                     _materialDatabase->GetMaterial(dicomData, x, y, z);
            }
        }
    }

    // Get list of materials to fulfill G4NestedParameterisation interface
    _materials = materialDatabase->GetAllMaterials();
}
