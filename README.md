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

Macro commands
--------------    

* /dicom/loadFiles - specify DICOM files to load (using glob syntax). Can be used multiple times.
* /dicom/readFiles - force reading of files. Not necessary because geometry building does this implicitely.

Classes
-------

Inspiration
-----------
https://github.com/christopherpoole/G4VoxelData