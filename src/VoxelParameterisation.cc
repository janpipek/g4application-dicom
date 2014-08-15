#include "VoxelParameterisation.hh"

#include <G4VTouchable.hh>
#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>

#include "VMaterialDatabase.hh"
#include "DicomData.hh"
#include "dicomConfiguration.hh"

using namespace g4dicom;
using namespace boost;
using namespace std;
using namespace g4;

G4Material* VoxelParameterisation::ComputeMaterial(G4VPhysicalVolume* currentVol, const G4int repNo, const G4VTouchable* parentTouch)
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
    G4Material* material = _voxelMaterials[x][y][z];
    if (_voxelsVisible)
    {
        G4VisAttributes attrs(_materialDatabase->GetColourMap()[material]);
        attrs.SetForceSolid(true);
        currentVol->GetLogicalVolume()->SetVisAttributes(attrs);
    }
    else
    {
        // Do nothing to prevent memory leaks (setting attributes creates copies and copies...)
        // currentVol->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
    }
    return material;
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
      _dicomData(dicomData),
      _colourMap(materialDatabase->GetColourMap())
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
    _voxelsVisible = Configuration::Get(VIS_SHOW_VOXELS, false);

    // Get list of materials to fulfill G4NestedParameterisation interface
    _materials = materialDatabase->GetAllMaterials();
}

void VoxelParameterisation::ConfigurationChanged(const string &key)
{
    if (key == VIS_SHOW_VOXELS)
    {
        _voxelsVisible = Configuration::Get<int>(key);
    }
}
