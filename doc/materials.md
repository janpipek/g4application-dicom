# Materials in DICOM plugin

## JSON format

## Examples

### materials-water.json

Very simple example material database interpreting all
tissues as water with varying density.

Air has constant density and is loaded from NIST database
It covers all HU under -900.

    {
        "name" : "air",
        "density" : 0.00125,
        "minHU" : -1050,
        "maxHU" : -899,
        "g4material" : "G4_AIR"
    },
 
Water is defined as a compound of H and O
It has varying density depending on HU.
Values are invented based on a real CT calibration.

    {
        "name" : "water",
        "minHU" : -900,
        "maxHU" : 3000,
        "densities" : [
            [-900, 0.19],
            [-500, 0.48],
            [-50, 0.96],
            [0, 1.00],
            [55, 1.05],
            [100, 1.45],
            [2000, 1.63],
            [3000, 2.23]
        ],
        "elements" : [
            [ 1, 2 ],
            [ 8, 1 ]
        ]
    }