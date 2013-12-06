#include "DicomPlugin.hh"

#include "G4Application.hh"

MAKE_G4_PLUGIN( DicomPlugin )

using namespace g4;
using namespace std;

DicomPlugin::DicomPlugin()
{
    // _geometryBuilder = new ExampleGeometryBuilder();
}

DicomPlugin::~DicomPlugin()
{
    // delete _geometryBuilder;
}