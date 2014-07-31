g4application-dicom
===================
DICOM plugin for g4application (see https://github.com/janpipek/g4application),
based on GDCM.

MIT License (see LICENSE file).

The plugin is in a VERY EARLY stage of development.

Requirements:
-------------
* g4application
* Geant4
* GDCM
* boost
* json-cpp (for reading material settings)
* C++11 capable compiler (gcc 4.8 tested)

Macro commands
--------------    
* /dicom/loadFiles - specify DICOM files to load (using glob syntax). Can be used multiple times.
* /dicom/readFiles - force reading of files. Not necessary because geometry building does this automatically.
* /dicom/crop - select only a rectangular subset of voxels. Provide 6 values (xmin, xmax, ymin, ymax, zmin, zmax) as voxel indices, 0 meaning no limit. Zero-based indexing, upper limit is not inclusive.
* /dicom/rotateX - rotate current setup around X axis
* /dicom/rotateY - rotate current setup around Y axis
* /dicom/rotateZ - rotate current setup around Z axis
* /dicom/resetRotation - reset rotation to the default setting

Configuration values
--------------------
* dicom.phantom_center_x
* dicom.phantom_center_y
* dicom.phantom_center_z
* dicom.materials.hu_step - step in HU units when generating table of materialss
* dicom.vis.showVoxels - whether to visualize individual voxels.
    Otherwise, only the surrounding box is displayed.

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
