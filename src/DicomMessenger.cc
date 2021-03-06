#include "DicomMessenger.hh"

#include <G4RotationMatrix.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

#include "ui/UIcmdWithNIntegers.hh"

#include "DicomComponent.hh"
#include "DicomReader.hh"

using namespace g4dicom;
using namespace g4;
using namespace std;

DicomMessenger::DicomMessenger(DicomComponent &component)
    : _component(component)
{
    // TODO: Add hints

    _addFilesCommand = new G4UIcmdWithAString("/dicom/addFiles", this);
    _readFilesCommand = new G4UIcmdWithoutParameter("/dicom/readFiles", this);

    _cropCommand = new UIcmdWithNIntegers("/dicom/crop", 6, this);
    _autoCropCommand = new G4UIcmdWithADouble("/dicom/autoCrop", this);

    _loadMaterialsCommand = new G4UIcmdWithAString("/dicom/loadMaterials", this);

    _rotateXCommand = new G4UIcmdWithADoubleAndUnit("/dicom/rotateX", this);
    _rotateXCommand->SetUnitCandidates("deg rad");

    _rotateYCommand = new G4UIcmdWithADoubleAndUnit("/dicom/rotateY", this);
    _rotateYCommand->SetUnitCandidates("deg rad");

    _rotateZCommand = new G4UIcmdWithADoubleAndUnit("/dicom/rotateZ", this);
    _rotateZCommand->SetUnitCandidates("deg rad");

    _resetRotationCommand = new G4UIcmdWithoutParameter("/dicom/resetRotation", this);
}

DicomMessenger::~DicomMessenger()
{
    delete _addFilesCommand;
    delete _readFilesCommand;
    delete _cropCommand;
    delete _autoCropCommand;
    delete _loadMaterialsCommand;

    delete _rotateXCommand;
    delete _rotateYCommand;
    delete _rotateZCommand;
    delete _resetRotationCommand;
}


G4String DicomMessenger::GetCurrentValue(G4UIcommand *command)
{
}

typedef G4RotationMatrix& (G4RotationMatrix::*rotationMethod)(G4double angle);

void DoRotation(DicomComponent& component, G4String newValue, rotationMethod method)
{
    double angle = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    G4RotationMatrix originalRotation = component.GetPhantomRotation();
    G4RotationMatrix newRotation = (originalRotation.*method)(-angle);
    component.SetPhantomRotation(newRotation);
}

void DicomMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == _addFilesCommand)
    {
        _component.GetDicomReader()->AddFiles(newValue);
    }
    else if (command == _readFilesCommand)
    {
        _component.GetDicomReader()->ReadFiles();
    }
    else if (command == _cropCommand)
    {
        vector<int> cropLimits = _cropCommand->GetNewIntVectorValue(newValue);
        _component.SetCropLimits(cropLimits);
    }
    else if (command == _autoCropCommand)
    {
        double minHU = _autoCropCommand->GetNewDoubleValue(newValue);
        _component.SetAutoCrop(minHU);
    }
    else if (command == _loadMaterialsCommand)
    {
        _component.LoadMaterialDatabase(newValue);
    }

    else if (command == _rotateXCommand)
    {
        DoRotation(_component, newValue, &G4RotationMatrix::rotateX);
    }
    else if (command == _rotateYCommand)
    {
        DoRotation(_component, newValue, &G4RotationMatrix::rotateY);
    }
    else if (command == _rotateZCommand)
    {
        DoRotation(_component, newValue, &G4RotationMatrix::rotateZ);
    }
    else if (command == _resetRotationCommand)
    {
        _component.SetPhantomRotation(G4RotationMatrix());
    }
}
