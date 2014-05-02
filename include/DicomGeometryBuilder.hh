#ifndef DICOMGEOMETRYBUILDER_HH
#define DICOMGEOMETRYBUILDER_HH

#include "GeometryBuilder.hh"

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

    private:
        DicomData* _data;
    };
}

#endif // DICOMGEOMETRYBUILDER_HH
