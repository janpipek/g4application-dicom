#ifndef DICOMGEOMETRYBUILDER_HH
#define DICOMGEOMETRYBUILDER_HH

#include "GeometryBuilder.hh"

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
    };
}

#endif // DICOMGEOMETRYBUILDER_HH
