#ifndef DICOMREADER_HH
#define DICOMREADER_HH

#include <string>
#include <vector>

namespace g4dicom
{
    class DicomData;

    /**
     * @short Class responsible for reading DICOM files
     *  and turning them into inner representation.
     */
    class DicomReader
    {
    public:
        DicomReader();

        virtual ~DicomReader();

        void AddFiles(const std::string& path);

        void ReadFiles();

        const DicomData* GetData();

        bool IsRead() const { return _data != 0; }

    private:
        std::vector<std::string> _paths;

        DicomData* _data;
    };
}

#endif // DICOMREADER_HH
