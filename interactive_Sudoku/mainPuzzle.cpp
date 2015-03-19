/*
 * Patrick Drumm
 * lab 5, cse20212
 * mainPuzzle.cpp is the driver for Puzzle.h. Together, this program reads in a puzzle file through the Puzzle nondefault constructor and then allows the user to interactively solve a sudoku/wordoku puzzle.
 */

#include <iostream>
#include "Puzzle.h"
#include <cstdlib>
using namespace std;

int main() {
	int puzzleIncomplete=1;
	Puzzle<int> sudoku("sampleSudoku.txt");	// create a sudoku vector of type int
	system("clear");	// clear screen
	cout << "Welcome to Sudoku!\nHere is your puzzle.\nYou may change the value of any cell that is currently a zero in the initial puzzle. All others, however, are fixed.\nValid values are 1-9. Enter -1 at any time to quit.\nGood luck!\n\n";
	while(puzzleIncomplete!=0){	// while the user has not yet won
		sudoku.print();		// display the puzzle
		puzzleIncomplete = sudoku.solvePiece();	// prompt the user to solve a piece of the sudoku
	}
}
