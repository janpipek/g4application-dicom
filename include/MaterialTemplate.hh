#ifndef MATERIALTEMPLATE_HH
#define MATERIALTEMPLATE_HH

#include <vector>
#include <string>
#include <map>

class G4Material;

namespace g4dicom
{
    /**
      * @short Template for construction of materials for MaterialDatabase.
      *
      * The composition and other properties of the material are defined,
      * only density varies.
      *
      * The composition can be specified using one of the three ways:
      * - set g4MaterialName (gets G4NistManager material of this name)
      * - set mixtureElements (create material from element fractions)
      * - set compoundElements (create material from number of atoms)
      *
      * Specifying more than one will result in an exception.
      */
    struct MaterialTemplate
    {
    public:
        /**
          * @short Basic name for the material.
          *
          * When creating G4Materials which have to be unique,
          * "_<HU value>" is appended.
          */
        std::string name;

        double minHU;

        /**
          * @short Maximum HU value for this template.
          */
        double maxHU;

        /**
          * @short (hu, density) points for density interpolation
          */
        std::map<int, double> densities;

        /**
          * @short Name of the material to obtained from G4NistManager.
          */
        std::string g4material;

        /**
          * @short (Z, fraction) map of elements in the material.
          */
        std::map<int, double> mixtureElements;

        /**
          * @short (Z, number of atoms) map of elements in the material.
          */
        std::map<int, int> compoundElements;

        /**
          * @short A complete set of materials to be used.
          *
          * @param step Segmentation step of the HU range.
          *
          * Only relevant after all attributes are correctly set.
          */
        std::map<int, G4Material*> CreateMaterials(int step) const;

        /**
          * @short Set uniform density over the whole HU range.
          *
          * This must be done after minHU and maxHU are set
          * because it relies on these two values.
          */
        void SetUniformDensity(double density);

    private:
        /**
          * @short Calculate density for a HU value using interpolation.
          */
        double GetDensity(int hu) const;

        /**
          * @short Validate consistency of the settings.
          *
          * Throws exceptions if there is a problem.
          * Automatically called from CreateMaterials().
          */
        void Validate() const;
    };
}
#endif