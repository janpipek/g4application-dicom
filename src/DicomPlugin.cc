#include "DicomPlugin.hh"

#include "DicomData.hh"
#include "MaterialDatabase.hh"
#include "dicomConfiguration.hh"
#include "DicomMessenger.hh"
#include "DicomReader.hh"
#include "MaterialJsonReader.hh"

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
    _materialDatabase = new MaterialDatabase();
    _messenger = new DicomMessenger(*this);
    SetConfigurationDefaults();
}

DicomPlugin::~DicomPlugin()
{
    delete _reader;
    delete _messenger;
    delete _geometryBuilder;
    delete _materialDatabase;
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
    _geometryBuilder->SetDicomData(data);
    _geometryBuilder->SetMaterialDatabase(_materialDatabase);
}

void DicomPlugin::SetCropLimits(const vector<int>& cropLimits)
{
    _cropLimits = new vector<int>();
    *_cropLimits = cropLimits;
}

void DicomPlugin::LoadMaterialDatabase(const std::string& path)
{
    MaterialJsonReader reader;
    auto templates = reader.LoadTemplates(path);
    for (auto it = templates.begin(); it != templates.end(); it++)
    {
        _materialDatabase->AddMaterialTemplate(*it);
    }
}
