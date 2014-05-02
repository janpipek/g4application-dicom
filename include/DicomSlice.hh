#ifndef DICOMSLICE_HH
#define DICOMSLICE_HH

#include <boost/multi_array.hpp>
#include <vector>

namespace g4dicom
{
    struct DicomSlice
    {
    public:
        typedef boost::multi_array<int16_t, 3> data_type;

        data_type data;

        std::vector<int> GetDimensions() const;
    };
}

#endif // DICOMSLICE_HH
