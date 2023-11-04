# OCR-Sudoku-Solver

This project is an OCR that lets you solve your sudoku grid with a simple photo of her.

## Dependencies

SDL 2 :

```bash
$ sudo apt install libsdl2-2.0-0
```

SDL2 ttf :
```bash
$ sudo apt install libsdl2-ttf-2.0-0
```

## Installation

```bash
$ make
```

## Usage

<font size = "3"> **Pre-processing :** </font>

```bash
$ cd src/preProcessing/

$ ./preProcessing [IMAGE DIR] [OPTIONS]
```
Options :
```bash

-all           ->      Do all the steps
-r             ->      Rotate and save the image by n degrees
-save          ->      Save all steps images
-verbose       ->      Print verbose
--help         ->      Show the help panel
```

<font size = "3"> **Sudoko solver :** </font>

```bash
$ cd src/sudokuSolver/

$ ./solver [Grid DIR] [OPTIONS]
```
Options :
```bash
-p             ->      Print input grid
-po            ->      Print output grid
-g <grid width (px)> <border ratio> <width border> <font ratio>
               ->      Generate output image
-verbose       ->      Print verbose
--help         ->      Show the help panel
```

<font size = "3"> **Neural network :** </font>

```bash
$ cd src/neuralNetwork/

$ ./network [OPTIONS]
```
Options :
```bash
-xorTrain  <number epoch> <batch size> <learning rate>
                    ->  Start xor training
-digitsTrain <number epoch> <batch size> <learning rate>
                    ->  Start digits training
-load <dir>         ->      Load a neural network
-test <dir> <input> ->      Test network with input
-verbose            ->      Print informations
-save               ->      Save neural network
-defaultNetSpec     ->      Load default network specification
-defaultTrainSpec   ->      Load default training specification
--help              ->      Show the help panel
```

## Clean

To clean all the files generated :

```bash
$ make clean
```

## Contributors

This project is carried out as part of the 3rd semester at Epita.

Team composition :

* Brian Perret (brian.perret@epita.fr)
* Thomas Morin (thomas.morin@epita.fr)
* Ronan Leboucher (ronan.leboucher@epita.fr)
* Victor Gardere (victor.gardere@epita.fr)

## License

This project is licensed under the MIT licence. You can consult the complete text of the licence in the file [LICENSE](LICENSE).
