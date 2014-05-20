#ifndef DICOMMESSENGER_HH
#define DICOMMESSENGER_HH

#include <G4UImessenger.hh>

class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

namespace g4
{
    class UIcmdWithNIntegers;    
}

namespace g4dicom
{
    class DicomPlugin;

    class DicomMessenger : public G4UImessenger
    {
    public:
        DicomMessenger(DicomPlugin& plugin);

        virtual ~DicomMessenger();

    // G4UImessenger interface
    public:
        virtual G4String GetCurrentValue(G4UIcommand *command);

        virtual void SetNewValue(G4UIcommand *command, G4String newValue);

    private:
        G4UIcmdWithAString* _loadMaterialsCommand;

        G4UIcmdWithAString* _addFilesCommand;

        G4UIcmdWithoutParameter* _readFilesCommand;

        g4::UIcmdWithNIntegers* _cropCommand;

        DicomPlugin& _plugin;
    };
}

#endif // DICOMMESSENGER_HH
