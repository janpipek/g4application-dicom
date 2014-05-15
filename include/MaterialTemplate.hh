#ifndef MATERIALTEMPLATE_HH
#define MATERIALTEMPLATE_HH

#include <vector>
#include <string>
#include <map>

class G4Material;

namespace g4dicom
{
    struct MaterialTemplate
    {
    public:
        std::string name;

        std::string g4MaterialName;

        double minHU;

        double maxHU;

        std::map<int, double> densities;

        std::map<int, double> mixtureElements;

        std::map<int, int> compoundElements;

        std::map<int, G4Material*> CreateMaterials(int step) const;

        double GetDensity(int hu) const;
    };
}
#endif