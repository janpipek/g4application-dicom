#include "DicomPlugin.hh"

#include "G4Application.hh"

using namespace g4;
using namespace std;
using namespace g4dicom;

MAKE_G4_PLUGIN( g4dicom::DicomPlugin )

DicomPlugin::DicomPlugin()
{
    // _geometryBuilder = new ExampleGeometryBuilder();
}

DicomPlugin::~DicomPlugin()
{
    // delete _geometryBuilder;
}
