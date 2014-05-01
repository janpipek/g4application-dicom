#ifndef DICOMPLUGIN_HH
#define DICOMPLUGIN_HH

// G4Application includes
#include "Plugin.hh"
#include "GeometryBuilder.hh"
#include "PhysicsBuilder.hh"

#include "DicomGeometryBuilder.hh"
#include "DicomMessenger.hh"
#include "DicomReader.hh"

namespace g4dicom
{
    class DicomPlugin : public g4::Plugin
    {
    public:
        DicomPlugin();

        ~DicomPlugin();

        virtual const std::string GetName() const { return "DICOM Plugin"; }

        virtual g4::GeometryBuilder* GetGeometryBuilder() { return _geometryBuilder; }

        virtual void OnGeometryInitializing();

    private:
        DicomGeometryBuilder* _geometryBuilder;

        DicomMessenger* _messenger;

        DicomReader* _reader;
    };
}

#endif // DICOMPLUGIN_HH
