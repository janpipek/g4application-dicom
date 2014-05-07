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

        /**
         * @brief [x, y, z] dimensions of all data.
         */
        std::vector<int> GetDimensions();

    private:
        std::vector<DicomSlice*> _slices;

        std::vector<int> _dimensions;

        int _validity;

        bool _sorted;

        /**
         * @brief Go through all slices and ensure that
         *    we can build voxel geometry out of these.
         */
        void Validate();

        /**
         * @brief Sort slice according to their z-axis.
         */
        void SortSlices();
    };
}

#endif // DICOMDATA_HH
