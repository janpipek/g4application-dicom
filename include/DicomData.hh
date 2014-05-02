#ifndef DICOMDATA_HH
#define DICOMDATA_HH

#include <vector>

namespace g4dicom
{
    class DicomSlice;

    /**
     * @short Data object that represents a set of DICOM images.
     */
    class DicomData
    {
    public:
        DicomData();

        virtual ~DicomData();

        void Add(DicomSlice* slice);

        bool IsValid();

        std::vector<int> GetDimensions();

    private:
        std::vector<DicomSlice*> _slices;

        std::vector<int> _dimensions;

        int _validity;

        void Validate();
    };
}

#endif // DICOMDATA_HH
