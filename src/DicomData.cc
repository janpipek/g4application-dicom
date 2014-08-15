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
    /* G4cout << "New data slice registered, dimensions: "
           << dims[0] << " x "
           << dims[1] << " x "
           << dims[2] << G4endl;*/
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
        int dimX, dimY, dimZ;
        if (_xmax)
        {
            dimX = _xmax;
        }
        else
        {
            dimX = _slices[0]->GetDimensions()[0];
        }
        dimX -= _xmin;

        if (_ymax)
        {
            dimY = _ymax;
        }
        else
        {
            dimY = _slices[0]->GetDimensions()[1];
        }
        dimY -= _ymin;

        if (_zmax)
        {
            dimZ = _zmax;
        }
        else
        {
            dimZ = _slices[0]->GetDimensions()[2];
            for (int i = 1; i < _slices.size(); i++)
            {
                dimZ += _slices[i]->GetDimensions()[2];
            }
        }
        dimZ -= _zmin;

        _dimensions.push_back(dimX);
        _dimensions.push_back(dimY);
        _dimensions.push_back(dimZ);
    }
    return _dimensions;
}

std::vector<int> DicomData::GetOriginalDimensions()
{
    int dimX = _slices[0]->GetDimensions()[0];
    int dimY = _slices[0]->GetDimensions()[1];
    int dimZ = _slices[0]->GetDimensions()[2];
    for (int i = 1; i < _slices.size(); i++)
    {
        dimZ += _slices[i]->GetDimensions()[2];
    }

    std::vector<int> dims;
    dims.push_back(dimX);
    dims.push_back(dimY);
    dims.push_back(dimZ);
    return dims;
}

std::vector<double> DicomData::GetCenter()
{
    if (!IsValid())
    {
        throw runtime_error("Dicom data are not valid.");
    }
    vector<int> dims = GetDimensions();
    vector<double> voxelSize = GetVoxelSize();
    vector<double> origin = _slices[0]->origin;

    int shift[] { _xmin, _ymin, _zmin };

    vector<double> center;
    for (int i = 0; i < 3; i++)
    {
        double posI = origin[i] + (2 * shift[i] + dims[i] - 1) * voxelSize[i] / 2.;
        center.push_back(posI);
    }
    return center;
}

DicomSlice::basic_type DicomData::GetValue(int x, int y, int z)
{
    return GetOriginalValue(x + _xmin, y + _ymin, z + _zmin);
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

void DicomData::Crop(std::vector<int> limits)
{
    _validity = 0;
    _dimensions.clear();
    _xmin = limits[0];
    _xmax = limits[1];
    _ymin = limits[2];
    _ymax = limits[3];
    _zmin = limits[4];
    _zmax = limits[5];
}

void DicomData::AutoCrop(double minHU)
{
    _validity = 0;
    _dimensions.clear();

    vector<int> dims = GetOriginalDimensions();

    _xmin = dims[0];
    _ymin = dims[1];
    _zmin = dims[2];

    _xmax = 0;
    _ymax = 0;
    _zmax = 0;

    for (int x = 0; x < dims[0]; x++)
    {
        for (int y = 0; y < dims[1]; y++)
        {
            for (int z = 0; z < dims[2]; z++)
            {
                double val = GetOriginalValue(x, y, z);
                if (val > minHU)
                {
                    if (x < _xmin) _xmin = x;
                    if (x > _xmax) _xmax = x;
                    if (y < _ymin) _ymin = y;
                    if (y > _ymax) _ymax = y;
                    if (z < _zmin) _zmin = z;
                    if (z > _zmax) _zmax = z;
                }
            }
        }
    }

    if (!((_xmax >= _xmin) && (_ymax >= _ymin) && (_zmax >= _zmin)))
    {
        throw runtime_error("No voxel meets the limit.");
    }
}

void DicomData::Validate()
{
    _validity = -1; // Assumed invalid
    if (!_sorted)
    {
        SortSlices();
    }
    if (!_slices.size())
    {
        throw runtime_error("Cannot validate empty set of files.");
    }
    std::vector<double> cosines0 = _slices[0]->directionCosines;
    std::vector<double> spacing0 = _slices[0]->spacing;
    std::vector<int> dims0 = _slices[0]->GetDimensions();

    DicomSlice* previous = 0;
    double difference = 0.0;

    // Check crop limits
    if (_xmin > _xmax) return;
    if (_ymin > _ymax) return;
    if (_zmin > _zmax) return;
    if (_xmin && _xmin == _xmax) return;
    if (_ymin && _ymin == _ymax) return;
    if (_zmin && _zmin == _zmax) return;
    if (_xmin < 0 || _ymin < 0 || _zmin < 0) return;
    if (_xmax > dims0[0]) return;
    if (_ymax > dims0[1]) return;
    if (_zmax > _slices.size()) return;

    // Check the slice are the same
    for (auto it = _slices.begin(); it != _slices.end(); it++)
    {
        DicomSlice* slice = *it;
        if (slice->directionCosines != cosines0) return;
        if (slice->spacing != spacing0) return;
        if (slice->GetDimensions() != dims0) return;

        // Cannot interpret more slices in one file
        if (slice->GetDimensions()[2] > 1) return;
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
