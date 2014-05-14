#include "DicomPlugin.hh"

#include "DicomData.hh"
#include "MaterialDatabase.hh"
#include "dicomConfiguration.hh"
#include "DicomMessenger.hh"
#include "DicomReader.hh"

using namespace g4;
using namespace std;
using namespace g4dicom;

MAKE_G4_PLUGIN( g4dicom::DicomPlugin )

DicomPlugin::DicomPlugin()
    : _cropLimits(0)
{
    CreateUiDirectory("/dicom/");
    _geometryBuilder = new DicomGeometryBuilder();
    _reader = new DicomReader();
    _messenger = new DicomMessenger(*this);
    SetConfigurationDefaults();
}

DicomPlugin::~DicomPlugin()
{
    delete _reader;
    delete _messenger;
    delete _geometryBuilder;
    if (_cropLimits)
    {
        delete _cropLimits;
    }
}

void DicomPlugin::OnGeometryInitializing()
{
    DicomData* data = _reader->GetData();
    if (_cropLimits)
    {
        data->Crop(*_cropLimits);
    }
    if (!data->IsValid())
    {
        G4Exception("DicomPlugin",
            "InvalidDicomData", FatalException,
            "Cannot interpret DICOM slices as a single voxel array."
        );
    }
    MaterialDatabase* db = new MaterialDatabase();
    _geometryBuilder->SetDicomData(data);
    _geometryBuilder->SetMaterialDatabase(db);
}

void DicomPlugin::SetCropLimits(const vector<int>& cropLimits)
{
    _cropLimits = new vector<int>();
    *_cropLimits = cropLimits;
}
