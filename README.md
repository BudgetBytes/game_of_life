
# Conway's Game of Life implementation with Raylib

> This software is unfinished

## Simulation mode
```console
$ ./game_of_life ./turing2x2.png
```
- Download one of the pattern at https://conwaylife.appspot.com/library/
- Most of the pattern are represented by 4x4 cells, you can change the dimension directly in the website before downloading the image, or you can adjust 'IMG_CELL_SIZE'

## Creative mode

```console
$ ./gol_empty_board create
```
- D + LEFT_MOUSE_BUTTON     -> Draw left diagonal
- D + RIGHT_MOUSE_BUTTON    -> Draw right diagonal
- H + LEFT_MOUSE_BUTTON     -> Draw horizontal line
- V + LEFT_MOUSE_BUTTON     -> Draw vertical line
- N                         -> Next generation

