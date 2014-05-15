#include "MaterialTemplate.hh"

#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <G4Material.hh>
#include <G4NistManager.hh>

using namespace boost;
using namespace g4dicom;
using namespace std;

/**
  * @short Linear interpolation of values
  */
double interpolate(double x1, double y1, double x2, double y2, double x)
{
    return y1 + (x - x1) / (x2 - x1) * (y2 - y1);
}

template<typename T> G4Material* createMaterial(const string& name, double density, map<int, T> amounts)
{
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Material* material = new G4Material(name, density, amounts.size());
    for (auto it = amounts.begin(); it != amounts.end(); it++)
    {
        G4Element* element = nistManager->FindOrBuildElement(it->first);
        material->AddElement(element, it->second);
    }
    return material;
}

map<int, G4Material*> MaterialTemplate::CreateMaterials(int step) const
{
    G4NistManager* nistManager = G4NistManager::Instance();
    map<int, G4Material*> result;

    for (int hu = minHU; hu <= maxHU; hu += step)
    {
        string newName = name + "_";
        newName += lexical_cast<string>(hu);
        G4Material* material = 0;

        double density = GetDensity(hu);

        if (g4MaterialName.size())
        {
            material = nistManager->BuildMaterialWithNewDensity(newName, g4MaterialName, density);
        }
        else if (mixtureElements.size())
        {
            material = createMaterial(newName, density, mixtureElements);
        }
        else if (compoundElements.size())
        {
            material = createMaterial(newName, density, mixtureElements);
        }
        result[hu] = material;
    }
    return result;
}

double MaterialTemplate::GetDensity(int hu) const
{
    auto it = densities.lower_bound(hu);  // >= hu
    if (it == densities.end())            // more than maximum HU
    {
        throw domain_error("Density cannot be obtained for HU=" + lexical_cast<string>(hu));
    }
    else if (it == densities.begin())
    {
        if (it->first == minHU)           // we hit exactly the minimum HU
        {
            return it->second;
        }
        else                              // below minimum HU
        {
            throw domain_error("Density cannot be obtained for HU=" + lexical_cast<string>(hu));
        }
    }
    else
    {
        double x2 = it->first;
        double y2 = it->second;
        it--;
        double x1 = it->first;
        double y1 = it->second;
        
        return interpolate(x1, y1, x2, y2, hu);
    }
}