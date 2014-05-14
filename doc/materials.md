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
        "hu_low" : -1050,
        "hu_high" : -900,
        "g4material" : "G4_AIR"
    }
 
Water is defined as a compound of H and O
It has varying density depending on HU.
Values are invented based on a real CT calibration.

    {
        "name" : "water",
        "hu_low" : -900,
        "hu_high" : 3000,
        "densities" : [
            [-800, 0.19],
            [-500, 0.48],
            [-50, 0.96],
            [0, 1.00],
            [55, 1.05],
            [100, 1.45],
            [2000, 1.63]
        ],
        "elements" : [
            [ 1, 2 ],
            [ 8, 1 ]
        ]       
    }