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
    SetHUStep(Configuration::Get<int>(MATERIALS_HU_STEP, 10));
}

G4Material* MaterialDatabase::GetDefaultMaterial()
{
    return _materials.begin()->second;
}

void MaterialDatabase::CreateMaterials()
{
    _colourMap.clear();
    for (auto tempIt = _templates.begin(); tempIt != _templates.end(); tempIt++)
    {
        auto materials = tempIt->second->CreateMaterials(_step);
        for (auto matIt = materials.begin(); matIt != materials.end(); matIt++)
        {
            _materials[matIt->first] = matIt->second;
        }
    }
    G4cout << "MaterialDatabase: " <<  _materials.size() << " materials created in total." << G4endl;
}

void MaterialDatabase::ConfigurationChanged(const std::string& key)
{
    if (key == MATERIALS_HU_STEP)
    {
        SetHUStep(Configuration::Get<int>(key));
    }
    if (key == VIS_WINDOW_MIN || key == VIS_WINDOW_MAX)
    {
        _colourMap.clear();
    }
}

void MaterialDatabase::SetHUStep(int step)
{
    _step = step;
    if (Configuration::Get<int>(MATERIALS_HU_STEP) != _step)
    {
        Configuration::Set(MATERIALS_HU_STEP, _step);
    }
}

std::map<G4Material*, G4Colour>& MaterialDatabase::GetColourMap()
{
    if (_colourMap.empty())
    {
        double min = Configuration::Get<double>(VIS_WINDOW_MIN);
        double max = Configuration::Get<double>(VIS_WINDOW_MAX);

        for (auto it = _materials.begin(); it != _materials.end(); it++)
        {
            double hu = it->first;
            G4Material* mat = it->second;

            double brightness = (hu - min) / (max - min);
            if (brightness < 0.) brightness = 0.;
            if (brightness > 1.) brightness = 1.;

            // G4cout << brightness << G4endl;

            _colourMap[mat] = G4Colour(brightness, brightness, brightness);
        }
    }
    return _colourMap;
}

G4Material* MaterialDatabase::GetMaterial(DicomData* data, int x, int y, int z)
{
    if (_materials.empty())
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
