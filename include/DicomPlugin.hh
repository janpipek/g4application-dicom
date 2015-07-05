#ifndef DICOMPLUGIN_HH
#define DICOMPLUGIN_HH

// G4Application includes
#include "Plugin.hh"

namespace g4dicom
{
    /**
      * @short The main class of the DICOM plugin for g4aplication.
      *
      * It is automatically loaded.
      */
    class DicomPlugin : public g4::Plugin
    {
    public:
        DicomPlugin();

        virtual ~DicomPlugin();
        
    public:
        virtual const std::vector<std::string> GetAvailableComponents() const;
        
        virtual g4::Component* GetComponent(const std::string& name);
    };
}

#endif // DICOMPLUGIN_HH
