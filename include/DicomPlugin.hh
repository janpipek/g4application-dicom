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
    class MaterialDatabase;

    /**
      * @short The main class of the DICOM plugin for g4aplication.
      *
      * It is automatically loaded.
      */
    class DicomPlugin : public g4::Plugin
    {
    public:
        DicomPlugin();

        ~DicomPlugin();

        virtual const std::string GetName() const { return "DICOM Plugin"; }

        virtual g4::GeometryBuilder* GetGeometryBuilder() { return &DicomGeometryBuilder::Instance(); }

        virtual DicomReader* GetDicomReader() const { return _reader; }

        virtual void LoadMaterialDatabase(const std::string& path);

        virtual void OnGeometryInitializing();

        virtual void SetCropLimits(const std::vector<int>& cropLimits);

    private:
        std::vector<int>* _cropLimits;

        MaterialDatabase* _materialDatabase;

        DicomGeometryBuilder* _geometryBuilder;

        DicomMessenger* _messenger;

        DicomReader* _reader;
    };
}

#endif // DICOMPLUGIN_HH
