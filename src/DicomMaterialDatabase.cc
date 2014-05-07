#include "DicomMaterialDatabase.hh"

#include <G4Material.hh>
#include <G4NistManager.hh>

using namespace g4dicom;

G4Material *DicomMaterialDatabase::GetMaterialHU(int hu)
{
    if (_materials.find(hu) == _materials.end())
    {
        G4NistManager* nistManager = G4NistManager::Instance();
        double density = primitiveHUtoDensity(hu);
        _materials[hu] = nistManager->BuildMaterialWithNewDensity("G4_WATER", density);
    }
    return _materials[hu];
}

G4Material* DicomMaterialDatabase::GetMaterial(DicomData* data, int x, int y, int z)
{
    return GetMaterialHU(data->GetValue(x, y, z));
}


double primitiveHUtoDensity(double hu)
{
    return 1.0 * g / cm3 * (hu + 1000.0) / 1000.0;
}
