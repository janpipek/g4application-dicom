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
    : _cropLimits(0), _autoCrop(false)
{
    CreateUiDirectory("/dicom/");
    _reader = new DicomReader();
    _materialDatabase = new MaterialDatabase();
    _messenger = new DicomMessenger(*this);
    SetConfigurationDefaults();
}

DicomPlugin::~DicomPlugin()
{
    delete _reader;
    delete _messenger;
    delete _materialDatabase;
    if (_cropLimits)
    {
        delete _cropLimits;
    }
}

void DicomPlugin::OnGeometryInitializing()
{
    DicomData* data = _reader->GetData();
    if (_autoCrop)
    {
        data->AutoCrop(_autoCropMinHU);
    }
    else if (_cropLimits)
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
    DicomGeometryBuilder::Instance().SetDicomData(data);
    DicomGeometryBuilder::Instance().SetMaterialDatabase(_materialDatabase);
}

void DicomPlugin::SetCropLimits(const vector<int>& cropLimits)
{
    _cropLimits = new vector<int>();
    *_cropLimits = cropLimits;
}

void DicomPlugin::SetAutoCrop(double minHU)
{
    _autoCrop = true;
    _autoCropMinHU = minHU;
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
