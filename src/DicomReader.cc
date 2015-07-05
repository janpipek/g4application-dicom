#include "DicomReader.hh"

#include <glob.h>

#include <globals.hh>

#include <gdcmImage.h>
#include <gdcmImageReader.h>
#include <gdcmPixelFormat.h>
#include <gdcmRescaler.h>

#include "DicomData.hh"
#include "DicomSlice.hh"

using namespace g4dicom;
using namespace std;

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
        int read = 0;
        _data = new DicomData();

        for (auto it = _paths.begin(); it != _paths.end(); it++)
        {
            glob_t res;
            int success = glob((*it).c_str(), 0, 0, &res);
            for (size_t i = 0; i < res.gl_pathc; i++)
            {
                gdcm::ImageReader reader;
                char* path = res.gl_pathv[i];
                // G4cout << "Reading DICOM file " << path << "..." << G4endl;
                reader.SetFileName(path);
                if (!reader.Read())
                {
                    G4Exception("DicomReader",
                        "FileCannotBeRead", FatalException,
                        (string("Cannot read DICOM file: ") + string(path)).c_str()
                    );
                }
                gdcm::Image image = reader.GetImage();
                DicomSlice* slice = GetSlice(&image);
                _data->Add(slice);
                read++;
            }
            globfree(&res);
        }
        G4cout << read << " files read." << G4endl;
    }
}

DicomData* DicomReader::GetData()
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
    slice->data.resize(boost::extents[y][x][z]);
    char* outBuffer = (char*)slice->data.data();

    // Prepare rescaler
    gdcm::Rescaler rescaler;
    rescaler.SetPixelFormat(originalFormat);
    rescaler.SetSlope(image->GetSlope());
    rescaler.SetIntercept(image->GetIntercept());
    rescaler.SetUseTargetPixelType(true);
    rescaler.SetTargetPixelType(int16format);
    rescaler.SetMinMaxForPixelType(int16format.GetMin(), int16format.GetMax());

    // Rescale (writes directly to data buffer of the slice)
    rescaler.Rescale(outBuffer, inBuffer, bufferLength);

    // Set other values
    const double* cosines = image->GetDirectionCosines();
    slice->directionCosines.assign(cosines, cosines + 6);

    const double* origin = image->GetOrigin();
    slice->origin.assign(origin, origin + 3);

    const double* spacing = image->GetSpacing();
    slice->spacing.assign(spacing, spacing + 3);

    return slice;
}
