#include "MaterialDatabase.hh"

#include <stdexcept>
#include <algorithm>
#include <boost/lexical_cast.hpp>

#include <G4Material.hh>
#include <G4NistManager.hh>

#include "DicomData.hh"
#include "dicomConfiguration.hh"

using namespace g4;
using namespace g4dicom;
using namespace std;
using namespace boost;

MaterialDatabase::MaterialDatabase()
{
    SetHUStep(Configuration::GetValue<int>(MATERIALS_HU_STEP, 10));
}

G4Material* MaterialDatabase::GetDefaultMaterial()
{
    return _materials.begin()->second;
}

void MaterialDatabase::CreateMaterials()
{
    for (auto tempIt = _templates.begin(); tempIt != _templates.end(); tempIt++)
    {
        auto materials = tempIt->second->CreateMaterials(_step);
        for (auto matIt = materials.begin(); matIt != materials.end(); matIt++)
        {
            _materials[matIt->first] = matIt->second;
        }
    }
}

void MaterialDatabase::ConfigurationChanged(const std::string& key)
{
    if (key == MATERIALS_HU_STEP)
    {
        SetHUStep(Configuration::GetValue<int>(key));
    }
}

void MaterialDatabase::SetHUStep(int step)
{
    _step = step;
    if (Configuration::GetValue<int>(MATERIALS_HU_STEP) != _step)
    {
        Configuration::SetValue(MATERIALS_HU_STEP, _step);
    }
}

G4Material* MaterialDatabase::GetMaterial(DicomData* data, int x, int y, int z)
{
    if (!_materials.size())
    {
        CreateMaterials();
    }
    int hu = data->GetValue(x, y, z);
    auto it = _materials.lower_bound(hu);
    if (it == _materials.end())
    {
        throw runtime_error("Material for HU=" 
            + lexical_cast<string>(hu)
            + " not found");
    }
    else
    {
        return it->second;
    }
}

vector<G4Material*> MaterialDatabase::GetAllMaterials()
{
    if (!_materials.size())
    {
        CreateMaterials();
    }
    vector<G4Material*> materials;
    for (auto it = _materials.begin(); it != _materials.end(); it++)
    {
        materials.push_back(it->second);
    }
    return materials;
}
