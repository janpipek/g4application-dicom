#include "DicomMessenger.hh"

using namespace g4dicom;

DicomMessenger::DicomMessenger(DicomReader &reader)
    : _reader(reader)
{
    _addFilesCommand = new G4UIcmdWithAString("/dicom/addFile", this);
    _readFilesCommand = new G4UIcmdWithoutParameter("/dicom/readFiles", this);
}

DicomMessenger::~DicomMessenger()
{
    delete _addFilesCommand;
    delete _readFilesCommand;
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
}
