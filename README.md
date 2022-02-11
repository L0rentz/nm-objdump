C program that has the same behavior as the command `nm` or `objdump -fs`.

## Requirements

 - [A Linux distribution](https://en.wikipedia.org/wiki/Linux_distribution)
 - [Make](https://www.gnu.org/software/make/)
 - [GCC](https://gcc.gnu.org/)

## Compilation

Navigate to the root of the repository from your Terminal and run `make` to build the `my_nm` and the `my_objdump` executable files.
You can run `make nm` to only build the `my_nm` executable file or `make objdump` to only build the `my_objdump` executable file.

## Running the program

To run the project, execute one of these commands from the root of the repository:
```
./my_nm [FILE NAME]
```
```
./my_objdump [FILE NAME]
```
