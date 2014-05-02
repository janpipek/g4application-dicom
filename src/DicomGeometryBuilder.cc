#include "DicomGeometryBuilder.hh"

#include "DicomData.hh"

using namespace g4dicom;

void g4dicom::DicomGeometryBuilder::BuildGeometry(G4LogicalVolume *)
{
    if (!_data)
    {
        // Throw an exception
    }
    vector<int> dims = _data->GetDimensions();
    G4cout << "Building DICOM voxel geometry of "
        << dims[0] << " x " << dims[1] << " x " << dims[2]
        << "..." << G4endl;
}


DicomGeometryBuilder::DicomGeometryBuilder()
    : _data(0)
{

}

DicomGeometryBuilder::~DicomGeometryBuilder()
{
    // _data is deleted by the originating DicomReader.
}
