#ifndef DICOMDATA_HH
#define DICOMDATA_HH

#include <vector>

#include "DicomSlice.hh"

namespace g4dicom
{
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

        /**
         * @brief Get HU value of a voxel.
         *
         * Automatically sorts and validates data if necessary.
         */
        DicomSlice::basic_type GetValue(int x, int y, int z);

        /**
         * @short Physical dimensions of one voxel.
         *
         * In Geant4 standard units (by coincidence mm's).
         */
        std::vector<double> GetVoxelSize();

        /**
         * @short Physical dimension of the whole volume.
         *
         * In Geant4 standard units (by coincidence mm's).
         */
        std::vector<double> GetTotalSize();

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
