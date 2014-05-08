#include "DicomMaterialDatabase.hh"

#include <G4Material.hh>
#include <G4NistManager.hh>

using namespace g4dicom;
using namespace std;

G4Material *VDicomMaterialDatabase::GetDefaultMaterial()
{
    return G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
}

G4Material* DicomMaterialDatabase::GetMaterialByHU(int hu)
{
    if (hu < minHU)
    {
        return GetDefaultMaterial();
    }
    if (_materials.find(hu) == _materials.end())
    {
        G4NistManager* nistManager = G4NistManager::Instance();
        double density = primitiveHUtoDensity(hu);
        _materials[hu] = nistManager->BuildMaterialWithNewDensity("G4_WATER", density);
    }
    return _materials[hu];
}

DicomMaterialDatabase::DicomMaterialDatabase()
{
    _materials[minHU] = GetDefaultMaterial();
}

G4Material* DicomMaterialDatabase::GetMaterial(DicomData* data, int x, int y, int z)
{
    return GetMaterialByHU(data->GetValue(x, y, z));
}

vector<G4Material*> DicomMaterialDatabase::GetAllMaterials()
{
    vector<G4Material*> materials;
    for (auto it = _materials.begin(); it != _materials.end(); it++)
    {
        materials.push_back(it->second);
    }
    return materials;
}

double primitiveHUtoDensity(double hu)
{
    return 1.0 * g / cm3 * (hu + 1000.0) / 1000.0;
}
