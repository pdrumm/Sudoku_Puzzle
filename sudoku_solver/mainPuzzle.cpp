/*
 * Patrick Drumm
 * lab 5, cse20212
 * This program is the main driver for Puzzle.h. This driver takes in a 9x9 sudoku puzzle (with zeros entered where all unsolved cells are located) and then uses the algorithms in Puzzle.h in an attampt to solve the puzzle. The driver outputs the filled in puzzle, with a message stating whether or not the sudoku has been solved.
 */

#include <iostream>
#include "Puzzle.h"
#include <cstdlib>
#include <fstream>
using namespace std;

int main() {
	int puzzleIncomplete=1;
	// read in a filename from the user. Do not run the program if the file does not exist
	ifstream inFile;	// object for reading from a file
	string puzName;
	cout << "\nPlease enter the full file name of the textfile containing the sudoku puzzle.\nFilename: ";
	cin >> puzName;
	inFile.open(puzName.c_str(), ios::in);	// opens the puzzlefile for input
	if(! inFile) {
		cout << "Error. File does not exist."<< endl;
		return 1;
	}
	inFile.close();
	// The puzzle exists, so start the solving process!
	Puzzle<int> sudoku(puzName);	// create a sudoku vector of type int
	system("clear");	// clear screen
	sudoku.getPosVals();	// create the 3d possible value vector for the given sudoku
	// display the initial sudoku puzzle to the user with no cells solved
	cout << "Here is the sudoku puzzle you entered.\n" << endl;
	sudoku.print();	// print the initial sudoku
	cout<<endl;
	// (attempt to)solve the sudoku!
	int changeMade=1;
	// this will continue to run until none of the algorithms can make any more changes to the sudoku (or the vector of each cells possible values)
	while(changeMade==1) {
		sudoku.singlePossibility();
		changeMade = sudoku.updatePosVals();
		// if the scanning algorithm has no more effect, try the singleton
		if(changeMade!=1) {
			sudoku.singleton();
			changeMade = sudoku.updatePosVals();
			// if neither the scanning or singleton algorithms have any effect, try the Patrick!
			if(changeMade!=1) {
				sudoku.Patrick();
				sudoku.singlePossibility();
				sudoku.singleton();
				changeMade = sudoku.updatePosVals();
			}
		}
	}
	// Print whether or not the puzzle has been solved, and then display the filled in sudoku
	cout << "\nAnd now here is the sudoku after the program's algorithms have been applied:" << endl;
	if(sudoku.didWin()==0)
		cout << "Your sudoku has been solved!\n" << endl;
	else
		cout << "Sorry, but your sudoku was not fully solved.\n" << endl;
	sudoku.print();	// print the solved sudoku
	cout << endl;
}
