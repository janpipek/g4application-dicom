#ifndef VMATERIALDATABASE_HH
#define VMATERIALDATABASE_HH

#include <vector>

class G4Material;

namespace g4dicom
{
    class DicomData;

    /**
     * @short Abstract class providing materials for voxels.
     *
     * A canonic implementation @see MaterialDatabase
     * is provided.
     */
    class VMaterialDatabase
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
}

#endif