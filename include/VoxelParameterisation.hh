#ifndef VOXELPARAMETERISATION_HH
#define VOXELPARAMETERISATION_HH

#include <vector>
#include <boost/multi_array.hpp>

#include <G4VNestedParameterisation.hh>
#include <G4Colour.hh>

#include <Configuration.hh>

namespace g4dicom
{
    class VMaterialDatabase;
    class DicomData;

    /**
     * @short Parameterization of voxel materials.
     */
    class VoxelParameterisation :
        public G4VNestedParameterisation,
        public g4::ConfigurationObserver
    {
    // G4VNestedParameterisation interface
    public:
        virtual G4Material *ComputeMaterial(G4VPhysicalVolume* currentVol,
                                            const G4int repNo, const G4VTouchable* parentTouch);

        virtual void ComputeTransformation(const G4int, G4VPhysicalVolume*) const;

        virtual G4int GetNumberOfMaterials() const;

        virtual G4Material* GetMaterial(G4int idx) const;

    public:
        VoxelParameterisation(VMaterialDatabase* materialDatabase, DicomData* dicomData);

    private:
        DicomData* _dicomData;

        std::map<G4Material*, G4Colour> _colourMap;

        VMaterialDatabase* _materialDatabase;

        boost::multi_array<G4Material*, 3> _voxelMaterials;

        std::vector<G4Material*> _materials;

        std::vector<int> _dims;

        double _zHalfSize;

        bool _voxelsVisible = false;

    // ConfigurationObserver interface
    protected:
        virtual void ConfigurationChanged(const string &key);
    };
}

#endif // VOXELPARAMETERISATION_HH
