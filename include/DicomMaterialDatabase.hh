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
        virtual G4Material* GetMaterial(DicomData* data, int x, int y, int z) = 0;
    };

    class DicomMaterialDatabase : public VDicomMaterialDatabase
    {
    public:
        virtual G4Material* GetMaterial(DicomData* data, int x, int y, int z);

    protected:
        virtual G4Material* GetMaterialHU(int hu);

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
