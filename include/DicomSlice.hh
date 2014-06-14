#ifndef DICOMSLICE_HH
#define DICOMSLICE_HH

#include <boost/multi_array.hpp>
#include <vector>

namespace g4dicom
{
    /**
     * @short Data from one DICOM file.
     *
     * Implemented using boost::multi_array.
     *
     * It is expected to hold data after reshaping using slope & intercept.
     * DicomSlices are stacked in @see DicomData.
     */
    struct DicomSlice
    {
    public:
        /**
         * @short Data type of one data element.
         */
        typedef int16_t basic_type;

        typedef boost::multi_array<basic_type, 3> data_type;

        data_type data;

        /**
          * @short Number of data elements along three principial axes.
          */
        std::vector<int> GetDimensions() const;

        /**
          * @short Total number of data elements.
          */
        int GetSize() const { return data.num_elements(); }

        /**
          * @short Origin as defined in DICOM.
          */
        std::vector<double> origin;

        /**
          * @short Spacing as defined in DICOM.
          */
        std::vector<double> spacing;

        /**
          * @short Direction cosines as defined in DICOM.
          */
        std::vector<double> directionCosines;
    };
}

#endif // DICOMSLICE_HH
