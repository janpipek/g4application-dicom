#include "DicomData.hh"

#include "DicomSlice.hh"

using namespace g4dicom;

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
    _slices.push_back(slice);
}
