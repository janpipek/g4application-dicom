#include "DicomSlice.hh"

using namespace g4dicom;
using namespace std;

vector<int> DicomSlice::GetDimensions() const
{
    vector<int> dimensions;
    auto shape = data.shape();
    dimensions.push_back(shape[0]);
    dimensions.push_back(shape[1]);
    dimensions.push_back(shape[2]);
    return dimensions;
}
