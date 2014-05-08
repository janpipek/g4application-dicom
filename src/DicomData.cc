#include "DicomData.hh"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include <globals.hh>

using namespace g4dicom;
using namespace std;

DicomData::DicomData()
    : _validity(0), _sorted(true), _slices(0), _dimensions(0)
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
    _validity = 0;
    _sorted = false;
    vector<int> dims = slice->GetDimensions();
    G4cout << "New data slice registered, dimensions: "
           << dims[0] << " x "
           << dims[1] << " x "
           << dims[2] << G4endl;
    _slices.push_back(slice);
}

bool DicomData::IsValid()
{
    if (_validity == 0)
    {
        Validate();
    }
    return (_validity > 0);
}

std::vector<int> DicomData::GetDimensions()
{
    if (_dimensions.size() == 0)
    {
        int dimX = _slices[0]->GetDimensions()[0];
        int dimY = _slices[0]->GetDimensions()[1];
        int dimZ = _slices[0]->GetDimensions()[2];
        for (int i = 1; i < _slices.size(); i++)
        {
            dimZ += _slices[i]->GetDimensions()[2];
        }
        _dimensions.push_back(dimX);
        _dimensions.push_back(dimY);
        _dimensions.push_back(dimZ);
    }
    return _dimensions;
}

DicomSlice::basic_type DicomData::GetValue(int x, int y, int z)
{
    if (!_validity)
    {
        Validate();
    }
    return _slices[z]->data[x][y][0];
}

vector<double> DicomData::GetVoxelSize()
{
    double x = _slices[0]->spacing[0] * mm;
    double y = _slices[0]->spacing[1] * mm;
    double z;
    if (_slices.size() > 1)
    {
        z = (_slices[1]->origin[2] - _slices[0]->origin[2]) * mm;
    }
    else
    {
        throw length_error("Cannot get the 3rd voxel dimension for 2D data.");
    }
    vector<double> size;
    size.push_back(x);
    size.push_back(y);
    size.push_back(z);
    return size;
}

vector<double> DicomData::GetTotalSize()
{
    vector<double> voxelSize = GetVoxelSize();
    vector<int> dims = GetDimensions();

    vector<double> totalSize;
    for (int i = 0; i < 3; i++)
    {
        totalSize.push_back(voxelSize[i] * dims[i]);
    }
    return totalSize;
}

void DicomData::Validate()
{
    _validity = -1; // Assumed invalid
    if (!_sorted)
    {
        SortSlices();
    }
    std::vector<double> cosines0 = _slices[0]->directionCosines;
    std::vector<double> spacing0 = _slices[0]->spacing;
    std::vector<int> dims0 = _slices[0]->GetDimensions();

    DicomSlice* previous = 0;
    double difference = 0.0;

    for (auto it = _slices.begin(); it != _slices.end(); it++)
    {
        DicomSlice* slice = *it;
        if (slice->directionCosines != cosines0) return;
        if (slice->spacing != spacing0) return;
        if (slice->GetDimensions() != dims0) return;

        // Cannot interpret more slices in one file
        if (slice->GetDimensions()[2] > 1);
        if (previous)
        {
            // All differences are same
            if (difference)
            {
                double lastDiff = slice->origin[2] - previous->origin[2];
                if (fabs(lastDiff - difference) > 0.001)
                {
                    return;
                }
            }
        }
        previous = slice;
    }
    G4cout << "DICOM data validated." << G4endl;
    _validity = 1; // If we got here => success
}

// Help function for the sorting algorithm
bool sortOrder(DicomSlice* slice1, DicomSlice* slice2)
{
    return slice1->origin[2] < slice2->origin[2];
}

void DicomData::SortSlices()
{
    G4cout << "Sorting DICOM slices..." << G4endl;
    sort(_slices.begin(), _slices.end(), sortOrder);
    _sorted = true;
}
