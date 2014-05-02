#include "DicomReader.hh"

#include <globals.hh>
#include <gdcmImageReader.h>
#include <gdcmPixelFormat.h>
#include <gdcmRescaler.h>

#include "DicomData.hh"
#include "DicomSlice.hh"

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
        G4Exception("DicomReader",
            "GeometryInitialized", FatalException,
            "Cannot add DICOM file, geometry already initialized."
        );
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
        _data = new DicomData();
        G4cout << "Reading DICOM files..." << G4endl;
        for (auto it = _paths.begin(); it != _paths.end(); it++)
        {
            gdcm::ImageReader reader;
            reader.SetFileName((*it).c_str());
            reader.Read();
            gdcm::Image image = reader.GetImage();
            DicomSlice* slice = GetSlice(&image);
            _data->Add(slice);
        }
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

DicomSlice *DicomReader::GetSlice(gdcm::Image *image)
{
    DicomSlice* slice = new DicomSlice();

    // Get all dimensions
    unsigned int noDims = image->GetNumberOfDimensions();
    const unsigned int* dims = image->GetDimensions();
    int x = dims[0];
    int y = dims[1];
    int z = 1;
    if (noDims == 3)
    {
        z = dims[2];
    }

    gdcm::PixelFormat originalFormat = image->GetPixelFormat();
    gdcm::PixelFormat int16format = (gdcm::PixelFormat)(gdcm::PixelFormat::INT16);

    // Prepare from where to read
    int bufferLength = image->GetBufferLength();
    char* inBuffer = new char[bufferLength];
    image->GetBuffer(inBuffer);

    // Prepare where to write
    slice->data.resize(boost::extents[x][y][z]);
    char* outBuffer = (char*)slice->data.data();

    // Prepare rescaler
    gdcm::Rescaler rescaler;
    rescaler.SetPixelFormat(originalFormat);
    rescaler.SetSlope(image->GetSlope());
    rescaler.SetIntercept(image->GetIntercept());
    rescaler.SetUseTargetPixelType(true);
    rescaler.SetTargetPixelType(int16format);
    rescaler.SetMinMaxForPixelType(int16format.GetMin(), int16format.GetMax());

    // Rescale
    rescaler.Rescale(outBuffer, inBuffer, bufferLength);
    return slice;
}
