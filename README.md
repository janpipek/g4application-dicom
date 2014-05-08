g4application-dicom
===================
DICOM plugin for g4application (see https://github.com/janpipek/g4application),
based on GDCM.

MIT License (see LICENSE file).

The plugin is in a VERY EARLY stage of development.

Requirements:
-------------
* g4application (possibility to build without it is planned)
* Geant4
* GDCM
* boost

Macro commands
--------------    
* /dicom/loadFiles - specify DICOM files to load (using glob syntax). Can be used multiple times.
* /dicom/readFiles - force reading of files. Not necessary because geometry building does this implicitely.

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
