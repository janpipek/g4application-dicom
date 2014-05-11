#include "DicomMessenger.hh"
#include "DicomReader.hh"

using namespace g4dicom;
using namespace g4;
using namespace std;

DicomMessenger::DicomMessenger(DicomReader &reader)
    : _reader(reader)
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
        _reader.AddFiles(newValue);
    }
    else if (command == _readFilesCommand)
    {
        _reader.ReadFiles();
    }
    else if (command == _cropCommand)
    {
        vector<int> cropFactors = _cropCommand->GetNewIntVectorValue(newValue);
        // TODO: use it
    }
}
