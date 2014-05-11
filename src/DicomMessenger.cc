#include "DicomMessenger.hh"
#include "DicomPlugin.hh"
#include "DicomReader.hh"

using namespace g4dicom;
using namespace g4;
using namespace std;

DicomMessenger::DicomMessenger(DicomPlugin &plugin)
    : _plugin(plugin)
{
    _addFilesCommand = new G4UIcmdWithAString("/dicom/addFiles", this);
    _readFilesCommand = new G4UIcmdWithoutParameter("/dicom/readFiles", this);
    _cropCommand = new UIcmdWithNIntegers("/dicom/crop", this);
}

DicomMessenger::~DicomMessenger()
{
    delete _addFilesCommand;
    delete _readFilesCommand;
    delete _cropCommand;
}

G4String DicomMessenger::GetCurrentValue(G4UIcommand *command)
{
}

void DicomMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == _addFilesCommand)
    {
        _plugin.GetDicomReader()->AddFiles(newValue);
    }
    else if (command == _readFilesCommand)
    {
        _plugin.GetDicomReader()->ReadFiles();
    }
    else if (command == _cropCommand)
    {
        vector<int> cropFactors = _cropCommand->GetNewIntVectorValue(newValue);
        // TODO: use it
    }
}
