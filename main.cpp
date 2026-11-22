#include<iostream>
#include<iomanip>
#include<string>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<ctime>
using namespace std;

typedef struct cell
{
	char status;							//'-'for no cell; 'X'for alive cell;
	int neighbor;							//counts the number of surrounding alive cells
	int generation;							//reords cell generation, default(no cell) = -1
};

int getInteger();				//This is a function for getting integers + handling errors
void welcome();
cell* createGrid(int rows, int cols);
void initGrid(cell* grid, int rows, int cols);
char* createMirror(int rows, int cols);
void syncMirror(cell* grid, char* mirror, int rows, int cols);
void writeGrid(cell* grid, ofstream &outfile, int rows, int cols, int gen);
int countNeighbors(char* mirror, int i, int j, int cols);
void updateGrid(cell* grid, char* mirror, int rows, int cols, int &born, int &died);
void end(int gen, int born, int died);

int main()
{
	welcome();

	ofstream outfile;
	outfile.open("Results.txt");
	if (!outfile.is_open())
	{
		cerr << "Failed to open file\n";
		return 1;
	}

	int rows = 0;
	int cols = 0;

	cout << "Now, please enter the length(columns) of your world: (integer)" << endl;
	//cols should be no larger than 100.
	do
	{
		cols = getInteger();
	} while (cols > 100 || cols <= 0);

	cout << "And the width(rows): (integer)" << endl;
	//rows should be no larger than 100.
	do
	{
		rows = getInteger();
	} while (rows > 100 || rows <= 0);

	cell* grid = createGrid(rows, cols);
	if (grid == NULL)
	{
		outfile.close();
		return 2;
	}

	initGrid(grid,rows,cols);

	char* mirror = createMirror(rows, cols);
	if (mirror == NULL)
	{
		outfile.close();
		free(grid);
		return 3;
	}

	syncMirror(grid, mirror, rows, cols);
	writeGrid(grid, outfile, rows, cols, 0);

	int gen = 0;
	cout << "Please enter the number of generations you would like to simulate: (integer)" << endl;
	//gen should be no more than 100.
	do
	{
		gen = getInteger();
	} while (gen > 100||gen <= 0);
	
	int born = 0;
	int died = 0;
	for (int n = 0; n < gen; n++)
	{
		updateGrid(grid, mirror, rows, cols, born, died);
		syncMirror(grid, mirror, rows, cols);
		writeGrid(grid, outfile, rows, cols, n+1);
	}

	free(grid);
	free(mirror);

	outfile << "In total, " << born << " cells were born.\n" << died << " cells died.\n";
	outfile.close();

	end(gen,born,died);
	return 0;
}

void welcome() {
	cout << "Welcome to the game of life." << endl
		<< "This is a simple simulation of cell growth." << endl;	
}

int getInteger()
{
	int value = 0;
	string line;
	while (true)
	{
		if (!getline(cin, line))
		{
			cerr << "Unexpected exit\n";
			exit(EXIT_FAILURE);
		}

		istringstream stream(line);

		stream >> value;
		if (!stream.fail())
		{
			stream >> ws;
			if(stream.eof())
				break;
		}
		cout << "Please enter again: a positive integer no more than 100\n" ;
	}
	return value;
}

cell* createGrid(int rows, int cols)
{
	cell* grid = (cell*)malloc(rows * cols * sizeof(cell));
	if (grid == NULL)
	{
		cerr << "Failed to create grid\n";
		return NULL;
	}
	return grid;
}

void initGrid(cell* grid ,int rows, int cols)
{
	srand((unsigned)time(nullptr));									//set random seed
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			double random_num = (double)rand() / (RAND_MAX + 1.0);	//ensuring r falls in [0,1)
			if (random_num > 0.5)
			{
				grid[i * cols + j].status = 'X';
				grid[i * cols + j].generation = 0;
				grid[i * cols + j].neighbor = 0;
			}
			else 
			{
				grid[i * cols + j].status = '-';
				grid[i * cols + j].generation = -1;
				grid[i * cols + j].neighbor = 0;
			}
		}
	}
}

char* createMirror(int rows, int cols)
{
	char* mirror = (char*)malloc((rows+2) * (cols+2) * sizeof(char));
	if (mirror == NULL)
	{
		cerr << "Failed to create mirror grid\n";
		return NULL;
	}
	for (int i = 0; i < rows+2; i++)
	{
		for (int j = 0; j < cols + 2; j++)
		{
			//Note that mirror_cols = cols + 2
			mirror[i * (cols+2) + j]= '-';
		}
	}

	return mirror;
}

void syncMirror(cell* grid, char* mirror, int rows, int cols)
{
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			//Note that mirror_cols = cols + 2
			mirror[(i + 1) * (cols + 2) + (j + 1)] = grid[i * cols + j].status;
		}
	}
}

void writeGrid(cell* grid, ofstream &outfile, int rows, int cols, int gen) 
{
	outfile << "Generation " << gen << " :\n";
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			outfile << grid[i * cols + j].status << setw(2);
		}
		outfile << '\n';
	}
	outfile << '\n';
}

//Note: i,j,and cols are the indexes of grid, not mirror.
int countNeighbors(char* mirror, int i, int j, int cols)
{
	int count = 0;
	if (mirror[(i+1) * (cols + 2) + (j+1)] == 'X')
	{
		count = -1;
	}
	for (int r = i; r <= i + 2; r++)
	{
		for (int s = j; s <= j + 2;s++)
		{
			if (mirror[r * (cols + 2) + s] == 'X')
				count++;
		}
	}
	return count;
}

void updateGrid(cell* grid, char* mirror, int rows, int cols, int &born, int&died)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i * cols + j].neighbor = countNeighbors(mirror, i, j, cols);

			//First look at neighbors
			if (grid[i * cols + j].neighbor < 2 || grid[i * cols + j].neighbor >= 4)
			{
				if (grid[i * cols + j].status == 'X')
				{
					died++;
					grid[i * cols + j].status = '-';
				}
				grid[i * cols + j].generation = -1;
			}
			else if (grid[i * cols + j].neighbor == 3)
			{
				if (grid[i * cols + j].status == '-')
				{
					born++;
					grid[i * cols + j].status = 'X';
				}
				if (grid[i * cols + j].generation >= 0)
					grid[i * cols + j].generation++;
				else
					grid[i * cols + j].generation = 0;
			}

			//Then look at generations
			if (grid[i * cols + j].generation > 10)
			{
				died++;
				grid[i * cols + j].status = '-';
				grid[i * cols + j].generation = -1;
			}
		}
	}
}

void end(int gen,int born, int died)
{
	cout << endl << "Your world existed for " << gen << " generations." << endl
		<< "During which " << born << " cells were born, and " << died << " cells died.\n"
		<< "But the beauty of life forever persists." << endl
		<< "This is one of the countless endings of the Game of Life." << endl
		<< "---------------------------------------------------------" << endl
		<< "To see how your world evolved, please exit the program and find the file 'Results.txt' in the same directory.\n";
}

