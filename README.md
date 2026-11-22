# Game of Life (Simplified)

A console-based C++ simulation of Conway's Game of Life with dynamic memory, mirrored grid, and generation tracking.

## Features

- Allows the user to enter the width and length of the grid, as well as the number of generations to simulate.
- Saves the results of each generation, along with the total number of cell deaths and births.
- Checks for the validity of user input, via the `getInteger()` function.

## Implementation

- Uses dynamic memory allocation, specifically using a single-dimensional array to simulate a two-dimensional grid.
- Creates a mirror grid for the update of each generation, which is larger than the actual grid by two rows and two columns.
- Each block of the grid stores the current status of cell (`'-'` for dead and `'X'` for alive), the number of alive neighbors, and the number of generations the cell has survived (`-1` for no cell).
- Updates the grid by iterating through the mirror grid, then synchronizes mirror grid and writes results into the file `"Results.txt"` per generation.

## Simulation Rules

- For each individual block, its initial status is randomly set to `'X'` (containing cell) or `'-'` (blank).
- In each generation, if the alive neighbors of a block is `< 2` or `>= 4`, the cell (if exists in that block) dies.
- Else, if the alive neighbors is `3`, then a new cell is born or the original cell sustains its life, depending on previous status.
- Additionally, if a live cell lives to more than 10 generations, it dies.

## Compile and Run 

Requires C++11 or later compilers.

```bash
g++ -std=c++11 -Wall -Wextra -o game_of_life main.cpp
./game_of_life
```

The output file, `"Results.txt"`, is saved in the same directory as the program.

## Project Structure

```bash
├── main.cpp        # The main function and other functions
├── Results.txt     # The results of the program, which is generated after execution.
└── README.md
```

## Notes

This is my first mini project. During the implementation of the program, I learned that using a single-dimensional array to represent two-dimensional arrays is easier to manage than two-dimensional arrays, though requiring more care in element iteration. Error checking is another issue I encountered, with the `failbit` set by the manipulator `ws` being a subtle error.

## Room for Improvement

- The filename of the simulation results is fixed. Future additions could be prompting the user for a name.
- Though characters in the grid are evenly spaced, the columns might still not appear as straight due to the use of proportional font.
- Other additions could be alive count, maximum count and figuring out the general pattern of cell behavior.
