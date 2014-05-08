#ifndef DICOMMATERIALDATABASE_HH
#define DICOMMATERIALDATABASE_HH

#include <map>

class G4Material;

#include "DicomData.hh"

namespace g4dicom
{
    /**
     * @short Abstract class providing materials for voxels.
     *
     * A canonic implementation @see DicomMaterialDatabase
     * is provided.
     */
    class VDicomMaterialDatabase
    {
    public:
        /**
         * @brief Material for a voxel.
         */
        virtual G4Material* GetMaterial(DicomData* data, int x, int y, int z) = 0;

        /**
         * @brief Material for empty voxels.
         */
        virtual G4Material* GetDefaultMaterial();

        /**
         * @brief All materials created by the database.
         *
         * Every possible value returned for the voxels
         * has to be included. This vector is used by
         * the nested parameterisation.
         */
        virtual std::vector<G4Material*> GetAllMaterials() = 0;
    };

    class DicomMaterialDatabase : public VDicomMaterialDatabase
    {
    public:
        DicomMaterialDatabase();

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

#endif // DICOMMATERIALDATABASE_HH
