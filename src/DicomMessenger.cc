#include "DicomMessenger.hh"

#include <G4RotationMatrix.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

#include "ui/UIcmdWithNIntegers.hh"

#include "DicomPlugin.hh"
#include "DicomReader.hh"
#include "DicomGeometryBuilder.hh"

using namespace g4dicom;
using namespace g4;
using namespace std;

DicomMessenger::DicomMessenger(DicomPlugin &plugin)
    : _plugin(plugin)
{
    // TODO: Add hints

    _addFilesCommand = new G4UIcmdWithAString("/dicom/addFiles", this);
    _readFilesCommand = new G4UIcmdWithoutParameter("/dicom/readFiles", this);
    _cropCommand = new UIcmdWithNIntegers("/dicom/crop", 6, this);
    _loadMaterialsCommand = new G4UIcmdWithAString("/dicom/loadMaterials", this);

    _rotateXPhantomCommand = new G4UIcmdWithADoubleAndUnit("/dicom/rotateXPhantom", this);
    _rotateXPhantomCommand->SetUnitCandidates("deg rad");

    _rotateYPhantomCommand = new G4UIcmdWithADoubleAndUnit("/dicom/rotateYPhantom", this);
    _rotateYPhantomCommand->SetUnitCandidates("deg rad");

    _rotateZPhantomCommand = new G4UIcmdWithADoubleAndUnit("/dicom/rotateZPhantom", this);
    _rotateZPhantomCommand->SetUnitCandidates("deg rad");
}

DicomMessenger::~DicomMessenger()
{
    delete _addFilesCommand;
    delete _readFilesCommand;
    delete _cropCommand;
    delete _loadMaterialsCommand;

    delete _rotateXPhantomCommand;
    delete _rotateYPhantomCommand;
    delete _rotateZPhantomCommand;
}


G4String DicomMessenger::GetCurrentValue(G4UIcommand *command)
{
}

typedef G4RotationMatrix& (G4RotationMatrix::*rotationMethod)(G4double angle);

void DoRotation(G4String newValue, rotationMethod method)
{
    double angle = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    G4RotationMatrix originalRotation = DicomGeometryBuilder::Instance().GetPhantomRotation();
    G4RotationMatrix newRotation = (originalRotation.*method)(angle);
    DicomGeometryBuilder::Instance().SetPhantomRotation(newRotation);
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
        vector<int> cropLimits = _cropCommand->GetNewIntVectorValue(newValue);
        _plugin.SetCropLimits(cropLimits);
    }
    else if (command == _loadMaterialsCommand)
    {
        _plugin.LoadMaterialDatabase(newValue);
    }

    else if (command == _rotateXPhantomCommand)
    {
        DoRotation(newValue, &G4RotationMatrix::rotateX);
    }
    else if (command == _rotateYPhantomCommand)
    {
        DoRotation(newValue, &G4RotationMatrix::rotateY);
    }
    else if (command == _rotateZPhantomCommand)
    {
        DoRotation(newValue, &G4RotationMatrix::rotateZ);
    }
}
