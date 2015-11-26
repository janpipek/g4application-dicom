#ifndef DICOMCONFIGURATION_HH
#define DICOMCONFIGURATION_HH

#include "Configuration.hh"

// This file contains all configuration values
// that are configurable using g4application
// standard configuration macros.

#define PHANTOM_CENTER_X "dicom.phantom_center_x"
#define PHANTOM_CENTER_Y "dicom.phantom_center_y"
#define PHANTOM_CENTER_Z "dicom.phantom_center_z"

#define PHANTOM_ROTATION_THETA "dicom.phantom_rotation_theta"
#define PHANTOM_ROTATION_PHI "dicom.phantom_rotation_phi"
#define PHANTOM_ROTATION_PSI "dicom.phantom_rotation_psi"

#define MATERIALS_HU_STEP "dicom.materials.hu_step"

#define VIS_SHOW_VOXELS "dicom.vis.showVoxels"
#define VIS_WINDOW_MIN "dicom.vis.windowMin"
#define VIS_WINDOW_MAX "dicom.vis.windowMax"

#define AS_PARALLEL_WORLD "dicom.parallel"

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

        g4::Configuration::SetDefaultValue(PHANTOM_ROTATION_THETA, 0.0);
        g4::Configuration::SetDefaultValue(PHANTOM_ROTATION_PHI, 0.0);
        g4::Configuration::SetDefaultValue(PHANTOM_ROTATION_PSI, 0.0);

        g4::Configuration::SetDefaultValue(MATERIALS_HU_STEP, 10);

        g4::Configuration::SetDefaultValue(VIS_SHOW_VOXELS, false);
        g4::Configuration::SetDefaultValue(VIS_WINDOW_MIN, -500.0);
        g4::Configuration::SetDefaultValue(VIS_WINDOW_MAX, 500.0);

        g4::Configuration::SetDefaultValue(AS_PARALLEL_WORLD, true);
    }
}

#endif // DICOMCONFIGURATION_HH
