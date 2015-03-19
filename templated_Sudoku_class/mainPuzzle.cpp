/*
 * Patrick Drumm
 * lab 5, cse20212
 * mainPuzzle.cpp is the driver for Puzzle.h. Together, this program reads in two puzzle files through the Puzzle nondefault constructor and then displays them to the user.
 */

#include <iostream>
#include "Puzzle.h"
using namespace std;

int main() {
	// create a Puzzle of type int
	Puzzle<int> sudoku("sampleSudoku.txt");
	cout << "\nSudoku:\n";
	sudoku.print();		// print the sudoku
	// create a Puzzle of type char
	cout << "\nWordoku:\n";
	Puzzle<char> wordoku("sampleWordoku.txt");
	wordoku.print();	// print the wordoku
	cout << endl;
}
