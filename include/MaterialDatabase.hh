#ifndef MATERIALDATABASE_HH
#define MATERIALDATABASE_HH

#include <map>

class G4Material;

#include "VMaterialDatabase.hh"
#include "DicomData.hh"

namespace g4dicom
{
    /**
      * @short Basic implemtation of a material database.
      */
    class MaterialDatabase : public VMaterialDatabase
    {
    public:
        MaterialDatabase();

        virtual G4Material* GetMaterial(DicomData* data, int x, int y, int z);

        /**
         * @brief HU value, under which the material is considered to be air.
         */
        const int minHU = -900;

        virtual std::vector<G4Material*> GetAllMaterials();

    protected:
        virtual G4Material* GetMaterialByHU(int hu);

    private:
        std::map<int, G4Material*> _materials;
    };

    /**
     * @short Primitive HU to density conversion.
     *
     * Linear scale (-1000 => 0 g.cm-3, 0 => 1 g.cm-3)
     */
    double primitiveHUtoDensity(double hu);
}

#endif // MATERIALDATABASE_HH
