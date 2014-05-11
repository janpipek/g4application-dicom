#ifndef DICOMPLUGIN_HH
#define DICOMPLUGIN_HH

// G4Application includes
#include "Plugin.hh"
#include "GeometryBuilder.hh"

#include "DicomGeometryBuilder.hh"

namespace g4dicom
{
    class DicomMessenger;
    class DicomReader;

    class DicomPlugin : public g4::Plugin
    {
    public:
        DicomPlugin();

        ~DicomPlugin();

        virtual const std::string GetName() const { return "DICOM Plugin"; }

        virtual g4::GeometryBuilder* GetGeometryBuilder() { return _geometryBuilder; }

        virtual DicomReader* GetDicomReader() const { return _reader; }

        virtual void OnGeometryInitializing();

        virtual void SetCropLimits(const std::vector<int>& cropLimits);

    private:
        std::vector<int>* _cropLimits;

        DicomGeometryBuilder* _geometryBuilder;

        DicomMessenger* _messenger;

        DicomReader* _reader;
    };
}

#endif // DICOMPLUGIN_HH
