#ifndef DICOMSLICE_HH
#define DICOMSLICE_HH

#include <boost/multi_array.hpp>
#include <vector>

namespace g4dicom
{
    /**
     * @short Data from one DICOM file.
     */
    struct DicomSlice
    {
    public:
        typedef boost::multi_array<int16_t, 3> data_type;

        data_type data;

        std::vector<int> GetDimensions() const;

        std::vector<double> origin;

        std::vector<double> spacing;

        std::vector<double> directionCosines;
    };
}

#endif // DICOMSLICE_HH
