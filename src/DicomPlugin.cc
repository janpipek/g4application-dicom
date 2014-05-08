#include "DicomPlugin.hh"

#include "DicomData.hh"
#include "DicomMaterialDatabase.hh"
#include "dicomConfiguration.hh"
#include "DicomMessenger.hh"
#include "DicomReader.hh"

using namespace g4;
using namespace std;
using namespace g4dicom;

MAKE_G4_PLUGIN( g4dicom::DicomPlugin )

DicomPlugin::DicomPlugin()
{
    CreateUiDirectory("/dicom/");
    _geometryBuilder = new DicomGeometryBuilder();
    _reader = new DicomReader();
    _messenger = new DicomMessenger(*_reader);
}

DicomPlugin::~DicomPlugin()
{
    delete _reader;
    delete _messenger;
    delete _geometryBuilder;
}

void DicomPlugin::OnGeometryInitializing()
{
    DicomData* data = _reader->GetData();
    if (!data->IsValid())
    {
        G4Exception("DicomPlugin",
            "InvalidDicomData", FatalException,
            "Cannot interpret DICOM slices as a single voxel array."
        );
    }
    DicomMaterialDatabase* db = new DicomMaterialDatabase();
    _geometryBuilder->SetDicomData(data);
    _geometryBuilder->SetMaterialDatabase(db);
}

void DicomPlugin::SetConfigurationDefaults()
{

}
