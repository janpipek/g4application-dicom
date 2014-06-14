#ifndef DICOMREADER_HH
#define DICOMREADER_HH

#include <string>
#include <vector>

namespace gdcm
{
    class Image;
}

namespace g4dicom
{
    class DicomData;
    class DicomSlice;

    /**
     * @short Class responsible for reading DICOM files
     *  and turning them into inner representation.
     */
    class DicomReader
    {
    public:
        DicomReader();

        virtual ~DicomReader();

        /**
          * @short Add a filename / glob pattern to be read.
          */
        void AddFiles(const std::string& path);

        void ReadFiles();

        /**
          * @short Get all data from read files.
          */
        DicomData* GetData();

        bool IsRead() const { return _data != 0; }

    protected:
        DicomSlice* GetSlice(gdcm::Image* image);

    private:
        std::vector<std::string> _paths;

        DicomData* _data;
    };
}

#endif // DICOMREADER_HH
