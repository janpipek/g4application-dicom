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
* /dicom/readFiles - force reading of files. Not necessary because geometry building does this implicitely.
* /dicom/crop - select only a rectangular subset of voxels. Provide 6 values (xmin, xmax, ymin, ymax, zmin, zmax) as voxel indices, 0 meaning no limit. Zero-based indexing, upper limit is not inclusive.

Configuration values
--------------------

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
