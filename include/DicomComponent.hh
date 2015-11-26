#ifndef DICOMCOMPONENT_HH
#define DICOMCOMPONENT_HH

#include "Component.hh"
#include "Configuration.hh"

class G4PVPlacement;

namespace g4dicom
{
    class DicomReader;
    class DicomMessenger;
    class DicomData;
    class VMaterialDatabase;

    /**
      * @brief The component for the voxel phantom.
      *
      * The voxel phantom is represented as a single box
      * that contains 2 levels of replicas (in x & y axes)
      * and parameterized volume in the third level (z axis).
      *
      * Materials are described using VoxelParameterisation
      * that is built from material database and DICOM data.
     */
    class DicomComponent : public g4::Component, private g4::ConfigurationObserver
    {
    public:
        DicomComponent();

        void ConfigurationChanged(const std::string& key) override;

        void BuildGeometry(G4LogicalVolume *logVolume) override;

        std::vector<G4VUserParallelWorld *> CreateParallelWorlds() override;

        bool CreateAsParallelWorld() const;

        virtual DicomReader* GetDicomReader() const { return _reader; }

        /**
          * @short Set DICOM data.
          *
          * You have to do this before building geometry
          * in run initialization.
          */
        void SetDicomData(DicomData* data) { _data = data; }

        virtual DicomData* GetDicomData() { return _data; }

        /**
         * @brief Read DICOM data from the reader.
         */
        virtual void LoadDicomData();

        /**
          * @short Set material database.
          */
        virtual void SetMaterialDatabase(VMaterialDatabase* database);

        /**
          * @short Load material database from a file.
          */
        virtual void LoadMaterialDatabase(const std::string& path);

        virtual void SetCropLimits(const std::vector<int>& cropLimits);

        void SetAutoCrop(double minHU);

        /**
          * @short Set the center of the voxel phantom.
          *
          * Can be done between runs -> geometry gets updated.
          * Similar effect can be achieved using configuration values
          *
          */
        void SetPhantomCenter(const G4ThreeVector& position);

        G4RotationMatrix GetPhantomRotation() const { return _rotationMatrix; }

        void SetPhantomRotation(const G4RotationMatrix& rotation);

        G4ThreeVector GetPhantomCenter() const { return _phantomCenter; }

        /**
          * @short Set whether individual voxels should be visible.
          */
        void SetVoxelsVisible(bool value);

        bool VoxelsVisible() const { return _voxelsVisible; }

    protected:      
        G4LogicalVolume* BuildLogicalVolume(DicomData* data);

        G4VPhysicalVolume* BuildPhysicalVolume(G4LogicalVolume *logVolume);

    private:
        std::vector<int>* _cropLimits;

        bool _autoCrop;

        double _autoCropMinHU;

        VMaterialDatabase* _materialDatabase;

        DicomMessenger* _messenger;

        DicomData* _data;

        DicomReader* _reader;

        bool _voxelsVisible = false;

        G4ThreeVector _phantomCenter;

        G4RotationMatrix _rotationMatrix; // Pointer to this will be used

        G4PVPlacement* _physContainer;
    };
}

#endif // DICOMCOMPONENT_HH

