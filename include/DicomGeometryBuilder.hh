#ifndef DICOMGEOMETRYBUILDER_HH
#define DICOMGEOMETRYBUILDER_HH

#include "GeometryBuilder.hh"
#include "DicomMaterialDatabase.hh"

namespace g4dicom
{
    class DicomData;

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

        void SetMaterialDatabase(VDicomMaterialDatabase* db)
        {
            _materialDatabase = db;
        }

    private:
        DicomData* _data;

        VDicomMaterialDatabase* _materialDatabase;
    };
}

#endif // DICOMGEOMETRYBUILDER_HH
