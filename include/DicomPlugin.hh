#ifndef DICOMPLUGIN_HH
#define DICOMPLUGIN_HH

// G4Application includes
#include "Plugin.hh"
#include "GeometryBuilder.hh"
#include "PhysicsBuilder.hh"

class DicomPlugin : public g4::Plugin
{
public:
    DicomPlugin();

    ~DicomPlugin();
    
    virtual const std::string GetName() const { return "DICOM Plugin"; }
    
    /* virtual g4::GeometryBuilder* GetGeometryBuilder() { return _geometryBuilder; } */
};

#endif // DICOMPLUGIN_HH
