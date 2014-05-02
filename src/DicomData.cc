#include "DicomData.hh"

#include <globals.hh>

#include "DicomSlice.hh"

using namespace g4dicom;
using namespace std;

DicomData::DicomData()
{

}

DicomData::~DicomData()
{
    for (auto it = _slices.begin(); it != _slices.end(); it++)
    {
        delete (*it);
    }
}

void DicomData::Add(DicomSlice *slice)
{
    vector<int> dims = slice->GetDimensions();
    G4cout << "New data slice registered, dimensions: "
           << dims[0] << " x "
           << dims[1] << " x "
           << dims[2] << G4endl;
    _slices.push_back(slice);
}
