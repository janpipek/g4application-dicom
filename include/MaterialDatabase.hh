#ifndef MATERIALDATABASE_HH
#define MATERIALDATABASE_HH

#include <map>
#include <string>

class G4Material;
#include "Configuration.hh"

#include "VMaterialDatabase.hh"
#include "DicomData.hh"
#include "MaterialTemplate.hh"

namespace g4dicom
{
    /**
      * @short Basic implementation of a material database.
      *
      * The HU range is covered per partes by different 
      * material "templates" that contain a specific chemical
      * composition and varying density (@see MaterialTemplate).
      */
    class MaterialDatabase : public VMaterialDatabase, g4::ConfigurationListener
    {
    public:
        G4Material* GetDefaultMaterial();

        virtual G4Material* GetMaterial(DicomData* data, int x, int y, int z);

        void AddMaterialTemplate(MaterialTemplate* temp)
        {
            _templates[temp->maxHU] = temp;
        }

        virtual void ConfigurationChanged(const std::string& key);

        virtual std::vector<G4Material*> GetAllMaterials();

    private:
        void CreateMaterials();

        int _step = 10;

        std::map<int, G4Material*> _materials;

        std::map<int, MaterialTemplate*> _templates;
    };
}

#endif // MATERIALDATABASE_HH
