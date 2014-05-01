#include "DicomReader.hh"

#include "globals.hh"

#include "DicomData.hh"

using namespace g4dicom;

DicomReader::DicomReader()
    : _data(0)
{

}

DicomReader::~DicomReader()
{
    if (_data)
    {
        delete _data;
    }
}

void DicomReader::AddFiles(const std::string &path)
{
    G4cout << "Adding DICOM files: " << path << G4endl;
    if (_data)
    {
        // TODO: throw exception
    }
    _paths.push_back(path);
}

void DicomReader::ReadFiles()
{
    if (_data)
    {
        return;
    }
    else
    {
        G4cout << "Reading DICOM files..." << G4endl;
        // TODO: Implement
    }
}

const DicomData* DicomReader::GetData()
{
    if (!_data)
    {
        ReadFiles();
    }
    return _data;
}
