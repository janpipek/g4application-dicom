#include "DicomPlugin.hh"

#include "DicomData.hh"

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
    const DicomData* data = _reader->GetData();
    _geometryBuilder->SetDicomData(data);
}
