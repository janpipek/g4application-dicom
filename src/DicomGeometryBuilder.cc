#include "DicomGeometryBuilder.hh"

#include "DicomData.hh"

using namespace g4dicom;

void g4dicom::DicomGeometryBuilder::BuildGeometry(G4LogicalVolume *)
{
    if (!_data)
    {
        // Throw an exception
    }
}


DicomGeometryBuilder::DicomGeometryBuilder()
    : _data(0)
{

}

DicomGeometryBuilder::~DicomGeometryBuilder()
{
    // _data is deleted by the originating DicomReader.
}
