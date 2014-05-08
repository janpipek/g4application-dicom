#ifndef DICOMCONFIGURATION_HH
#define DICOMCONFIGURATION_HH

#include "Configuration.hh"

// This file contains all configuration values
// that are configurable using g4application
// standard configuration macros.

#define PHANTOM_CENTER_X "dicom.phantom_center_x"
#define PHANTOM_CENTER_Y "dicom.phantom_center_y"
#define PHANTOM_CENTER_Z "dicom.phantom_center_z"

namespace g4dicom
{
    /**
     * @short Provide default values for all configuration options
     * of the plugin.
     *
     * It is called in the plugin initialization.
     */
    inline void SetConfigurationDefaults()
    {
        g4::Configuration::SetDefaultValue(PHANTOM_CENTER_X, 0.0);
        g4::Configuration::SetDefaultValue(PHANTOM_CENTER_Y, 0.0);
        g4::Configuration::SetDefaultValue(PHANTOM_CENTER_Z, 0.0);
    }
}

#endif // DICOMCONFIGURATION_HH
