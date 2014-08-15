#ifndef MATERIALDATABASE_HH
#define MATERIALDATABASE_HH

#include <map>
#include <string>

class G4Material;

#include "Configuration.hh"

#include "VMaterialDatabase.hh"
#include "MaterialTemplate.hh"

namespace g4dicom
{
    class DicomData;

    /**
      * @short Basic implementation of a material database.
      *
      * The HU range is covered per partes by different 
      * material "templates" that contain a specific chemical
      * composition and varying density (@see MaterialTemplate).
      */
    class MaterialDatabase : public VMaterialDatabase, g4::ConfigurationObserver
    {
    public:
        MaterialDatabase();

        /**
          * @short Default material equivalent to the material with lowest HU 
          *      in the database.
          */
        G4Material* GetDefaultMaterial();

        virtual G4Material* GetMaterial(DicomData* data, int x, int y, int z);

        void AddMaterialTemplate(MaterialTemplate* temp)
        {
            _templates[temp->maxHU] = temp;
        }

        virtual void ConfigurationChanged(const std::string& key);

        virtual std::vector<G4Material*> GetAllMaterials();

        /**
          * @short Set the step of HU range segmentation.
          *
          * It can be equivalently set using Configuration under
          * the key dicom.materials.hu_step.
          */
        void SetHUStep(int step);

        int GetHUStep() const { return _step; }

        virtual std::map<G4Material*, G4Colour>& GetColourMap();

    private:
        void CreateMaterials();

        int _step = 10;

        std::map<G4Material*, G4Colour> _colourMap;

        std::map<int, G4Material*> _materials;

        std::map<int, MaterialTemplate*> _templates;
    };
}

#endif // MATERIALDATABASE_HH
