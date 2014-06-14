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

/**
  * @short Create material from amounts (int => compound, double => mixture).
  *
  * Template takes advantage of G4Material::AddElement being overloaded.
  */
template<typename T> G4Material* createMaterial(const string& name, double density, map<int, T> amounts)
{
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Material* material = new G4Material(name, density, amounts.size());
    for (auto it = amounts.begin(); it != amounts.end(); it++)
    {
        G4Element* element = nistManager->FindOrBuildElement(it->first);
        if (!element)
        {
            throw runtime_error("Cannot add element with z=" + lexical_cast<string>(it->first));
        }
        material->AddElement(element, it->second);
    }
    return material;
}

void MaterialTemplate::Validate() const
{
    if (minHU > maxHU) throw runtime_error("Max HU must be > min HU.");
    // Three ways of specification. How many are used?
    int matSpecs = !!(g4material.size()) + !!(mixtureElements.size()) + !!(compoundElements.size());
    if (matSpecs != 1)
    {
        throw runtime_error("Must specify exactly one ("
            + lexical_cast<string>(matSpecs)
            + " used): G4 material name; element fractions; number of atoms."); 
    }
    if (densities.begin()->first != minHU) {
        throw runtime_error("First density entry must be equal to minimum HU ("
            + lexical_cast<string>(minHU)
            + ")");
    }
    if (densities.rbegin()->first != maxHU) {
        throw runtime_error("Last density entry must be equal to maximum HU ("
            + lexical_cast<string>(minHU)
            + ")");        
    }
}

map<int, G4Material*> MaterialTemplate::CreateMaterials(int step) const
{
    Validate();

    G4NistManager* nistManager = G4NistManager::Instance();
    map<int, G4Material*> result;

    for (int hu = minHU; hu <= maxHU; hu += step)
    {
        string newName = name + "_";
        newName += lexical_cast<string>(hu);
        
        double density = GetDensity(hu) * g/cm3;

        if (g4material.size())
        {
            result[hu] = nistManager->BuildMaterialWithNewDensity(newName, g4material, density);
        }
        else if (mixtureElements.size())
        {
            result[hu] = createMaterial(newName, density, mixtureElements);
        }
        else if (compoundElements.size())
        {
            result[hu] = createMaterial(newName, density, compoundElements);
        }
        /* G4cout << "Created material " << newName << "." << G4endl; */
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

void MaterialTemplate::SetUniformDensity(double density)
{
    densities.clear();
    densities[minHU] = density;
    densities[maxHU] = density;
}