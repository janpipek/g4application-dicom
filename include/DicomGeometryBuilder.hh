#ifndef DICOMGEOMETRYBUILDER_HH
#define DICOMGEOMETRYBUILDER_HH

#include "GeometryBuilder.hh"
#include "Configuration.hh"

class G4LogicalVolume;

namespace g4dicom
{
    class DicomData;
    class VMaterialDatabase;

    class DicomGeometryBuilder : public g4::GeometryBuilder
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
        void SetDicomData(DicomData* data)
        {
            _data = data;
        }

        void SetMaterialDatabase(VMaterialDatabase* db)
        {
            _materialDatabase = db;
        }

        G4LogicalVolume* BuildLogicalVolume();

    private:
        DicomData* _data;

        VMaterialDatabase* _materialDatabase;

        bool _voxelsVisible = false;
    };
}

#endif // DICOMGEOMETRYBUILDER_HH
