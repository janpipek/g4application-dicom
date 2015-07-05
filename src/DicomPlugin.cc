#include "DicomPlugin.hh"

#include "DicomData.hh"
#include "MaterialDatabase.hh"
#include "dicomConfiguration.hh"
#include "DicomMessenger.hh"
#include "DicomReader.hh"
#include "MaterialJsonReader.hh"
#include "DicomComponent.hh"

using namespace g4;
using namespace std;
using namespace g4dicom;

MAKE_G4_PLUGIN( g4dicom::DicomPlugin )

DicomPlugin::DicomPlugin()
{
    // CreateUiDirectory("/dicom/");
    SetConfigurationDefaults();
}

DicomPlugin::~DicomPlugin()
{
}

const vector<string> DicomPlugin::GetAvailableComponents() const
{
    return vector<string> { "default" };
}

Component *DicomPlugin::GetComponent(const string& name)
{
    if (name == "default")
    {
        return new DicomComponent();
    }
    else
    {
        return nullptr;
    }
}
