# OCR-Sudoku-Solver

This project is an OCR that lets you solve your sudoku grid with a simple photo of her.

Website : https://thegostsniperfr.github.io/Sudoku-Forgers/

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

<font size = "3"> **Main commands :** </font>

```bash
Usage : ./main [OPTIONS]

-gui                       ->      Open GUI
-ocr [dir] [option]        ->      Solve the image grid
```
<font size = "3"> **Options :** </font>

```bash
-save                      ->      Save intermediate steps
-verbose                   ->      Print verbose
--help                     ->      Show the help panel
```

<font size = "4"> **Sub-application commands :** </font>

<font size = "3"> **Solver app :** </font>

```bash
Usage : ./main -solver [Grid DIR] [OPTIONS]

-p             ->      Print input grid
-po            ->      Print output grid
-g <grid width (px)> <border ratio> <width border> <font ratio>
               ->      Generate output image
-verbose       ->      Print verbose
--help         ->      Show the help panel
```

<font size = "3"> **Neural network app :** </font>

```bash
Usage : ./main -nn [OPTIONS]

-xorTrain  <number epoch> <batch size> <learning rate>
                         ->      Start xor training
-digitsTrain <number epoch> <dataset range> <batch size> <learning rate>
                         ->      Start digits training
-p <number of hidden layers> <number of neurons per hidden layer>
                         ->      Network parameters
-defaultNetSpec          ->      Load default network spec
-defaultTrainSpec        ->      Load default training spec
-verbose                 ->      Print informations
-save                    ->      Save neural network
-testXor <dir> <index>   ->      Test xor network
-testDigit <dir> <index> ->      Test digit network
--help                   ->      Show the help panel
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
