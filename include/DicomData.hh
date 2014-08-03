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

        /**
         * @brief Is the data set valid?
         *
         * Lazy evaluation.
         */
        bool IsValid();

        /**
         * @brief [x, y, z] dimensions of all data.
         */
        std::vector<int> GetDimensions();

        /**
         * @brief Get the center of DICOM volume.
         *
         * Calculated from "origin" values stored in the DICOM files.
         * In Geant4 standard units.
         */
        std::vector<double> GetCenter();

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

        /**
         * @short Limit the number of voxels in each direction.
         *
         * @param limits 6-element vector xmin, xmax, ymin, ymax, zmin, zmax
         *      (exclusive)
         *
         * All 6 value have to be supplied. Zeroes mean no limit though.
         */
        void Crop(std::vector<int> limits);

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

        // Limits
        int _xmin = 0;
        int _xmax = 0;
        int _ymin = 0;
        int _ymax = 0;
        int _zmin = 0;
        int _zmax = 0;
    };
}

#endif // DICOMDATA_HH
