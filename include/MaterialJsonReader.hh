#ifndef MATERIALJSONREADER_HH
#define MATERIALJSONREADER_HH

#include <vector>
#include <string>

namespace g4dicom
{
    class MaterialTemplate;

    /**
     * @short A class that loads material templates from a JSON file.
     *
     * @see MaterialTemplate
     */
    class MaterialJsonReader
    {
    public:
        std::vector<MaterialTemplate*> LoadTemplates(const std::string& path);
    };
}
#endif
