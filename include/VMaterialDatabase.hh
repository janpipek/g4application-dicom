#ifndef VMATERIALDATABASE_HH
#define VMATERIALDATABASE_HH

#include <vector>

#include <G4NistManager.hh>
#include <G4Colour.hh>

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
         * @short Material for a voxel.
         */
        virtual G4Material* GetMaterial(DicomData* data, int x, int y, int z) = 0;

        /**
         * @short Material for empty voxels.
         */
        virtual G4Material* GetDefaultMaterial()
        {
            return G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
        }

        /**
         * @short All materials created by the database.
         *
         * Every possible value returned for the voxels
         * has to be included. This vector is used by
         * the nested parameterisation.
         */
        virtual std::vector<G4Material*> GetAllMaterials() = 0;

        /**
         * @short A map of colours for visualization of voxel materials.
         */
        virtual std::map<G4Material*, G4Colour>& GetColourMap() = 0;
    };
}

#endif
