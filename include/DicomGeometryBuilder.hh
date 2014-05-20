#ifndef DICOMGEOMETRYBUILDER_HH
#define DICOMGEOMETRYBUILDER_HH

#include <G4ThreeVector.hh>

#include "GeometryBuilder.hh"
#include "Configuration.hh"

class G4LogicalVolume;
class G4PVPlacement;

namespace g4dicom
{
    class DicomData;
    class VMaterialDatabase;

    /**
      * @short Builder of the voxel phantom geometry.
      *
      * The voxel phantom is represented as a single box
      * that contains 2 levels of replicas (in x & y axes)
      * and parameterized volume in the third level.
      *
      * Materials are described using VoxelParameterisation
      * that is built from material database and DICOM data.
      */
    class DicomGeometryBuilder : public g4::GeometryBuilder, g4::ConfigurationListener
    {
    public:
        DicomGeometryBuilder();

        ~DicomGeometryBuilder();

        virtual void BuildGeometry(G4LogicalVolume *);

        virtual void ConfigurationChanged(const std::string& key);

        bool VoxelsVisible() const
        {
            return _voxelsVisible;
        }

        /**
          * @short Set whether individual voxels should be visible.
          */ 
        void SetVoxelsVisible(bool value);

        /**
          * @short Set DICOM data.
          *
          * You have to do this before building geometry
          * in run initialization.
          */
        void SetDicomData(DicomData* data)
        {
            _data = data;
        }

        /**
          * @short Set material database.
          *
          * You have to do this before building geometry
          * in run initialization.
          */ 
        void SetMaterialDatabase(VMaterialDatabase* db)
        {
            _materialDatabase = db;
        }

        /**
          * @short Set the center of the voxel phantom.
          *
          * Can be done between runs -> geometry gets updated.
          * Similar effect can be achieved using configuration values
          * 
          */
        void SetPhantomCenter(const G4ThreeVector& position);

        G4ThreeVector GetPhantomCenter() const { return _phantomCenter; }

        G4LogicalVolume* BuildLogicalVolume();

    private:
        DicomData* _data;

        VMaterialDatabase* _materialDatabase;

        bool _voxelsVisible = false;

        G4ThreeVector _phantomCenter;

        G4PVPlacement* _physContainer;
    };
}

#endif // DICOMGEOMETRYBUILDER_HH
