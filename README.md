g4application-dicom
===================
DICOM plugin for g4application (see https://github.com/janpipek/g4application),
based on GDCM.

MIT License (see LICENSE file).

The plugin is in an EARLY stage of development.

Requirements:
-------------
* g4application
* Geant4
* GDCM
* boost
* json-cpp (for reading material settings)
* C++11 capable compiler (gcc 4.8 tested)

Features
--------
* Loading DICOM files (Note: tested for CT images with standard orientation and one slice per file, others may cause problems)
* Construction of voxel geometry
* Translation and rotation of the geometry
* Tools for creating materials (defined in external JSON)

Macro commands
--------------    
* /dicom/loadFiles - specify DICOM files to load (using glob syntax). Can be used multiple times.
* /dicom/readFiles - force reading of files. Not necessary because geometry building does this automatically.
* /dicom/loadMaterials - load materials from a JSON file (path relative to current working directory)
* /dicom/crop - select only a rectangular subset of voxels. Provide 6 values (xmin, xmax, ymin, ymax, zmin, zmax) as voxel indices, 0 meaning no limit. Zero-based indexing, upper limit is not inclusive.
* /dicom/autoCrop - crop the volume to the smallest rectangular box that contains all voxels with Hounsfield value larger than the limit (though in most cases manual setting seems better)
* /dicom/rotateX - rotate current setup around X axis
* /dicom/rotateY - rotate current setup around Y axis
* /dicom/rotateZ - rotate current setup around Z axis
* /dicom/resetRotation - reset rotation to the default setting

Note: The rotation commands rotate the volume itself, not the coordinate
frames as standard G4PVPlacement.

Configuration values
--------------------
* dicom.phantom_center_x (in mm)
* dicom.phantom_center_y (in mm)
* dicom.phantom_center_z (in mm)
* dicom.materials.hu_step - step in HU units when generating table of materials
* dicom.vis.showVoxels - whether to visualize individual voxels.
    Otherwise, only the surrounding box is displayed.
* dicom.phantom_rotation_theta - Euler angle theta (in deg)
* dicom.phantom_rotation_phi - Euler angle phi (in deg)
* dicom.phantom_rotation_psi - Euler angle psi (in deg)

Important Classes
-----------------
* DicomReader - utility that reads DICOM files using GDCM
* DicomSlice - data from one particular DICOM file (independent of GDCM)
* DicomData - a set of slices with included sorting and validation
* DicomPlugin - the plugin class for g4application
* (V)DicomMaterialDatabase - (abstract) class that provides material info
    for each pixel
* DicomGeometryBuilder - class that builds the voxel geometry for g4application
* VoxelParameterisation - G4VNestedParameterisation for the voxel phantom

Inspiration
-----------
* https://github.com/christopherpoole/G4VoxelData by Christopher Poole
* the official Geant4 DICOM extended example.
