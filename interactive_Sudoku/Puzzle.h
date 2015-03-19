/*
 * Patrick Drumm
 * lab 5, cse20212
 */

#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

template<typename T>
class Puzzle {
	public:
		Puzzle(string="sampleSudoku.txt");	// constructor
		void print();		// prints the current puzzle board
		int solvePiece();	// gives the user an attempt at solving a sodoku coordinate
		int validEntry(int,int,T);	// returns 1 if the inputted coordinate and value is a valid solution for the puzzle, or 2-4 as error codes for invalid values
		int didWin();		// returns 0 if the most recent entry has solved the puzzle!
	private:
		vector<vector<T> > puzMatrix;		// 2d vector stores the updated puzzle
		vector<vector<T> > origSkeletonMatrix;	// 2d vector stores the original puzzle
		ifstream inFile;	// object for reading from a file
};

#endif

// constructor opens file and stores the sudoku in a 2d vector
template<typename T>
Puzzle<T>::Puzzle(string puzzleFile) {
	inFile.open(puzzleFile.c_str(), ios::in);	// opens the puzzlefile for input
	T tempVar;	// temporary variable used to store puzzle values
	vector<T> tempVect;	// temporary vector stores each row one at a time
	for(int i=0; i<9; i++) {
		tempVect.clear();	// clear the previous row out
		for(int j=0; j<9; j++) {
			inFile >> tempVar;	// read in row i one variable at a time
			tempVect.push_back(tempVar);	// add the row variables to tempVect
		}
		// add row i to the puzzle
		puzMatrix.push_back(tempVect);
		// store original puzzle to the skeleton
		origSkeletonMatrix.push_back(tempVect);
	}
	inFile.close();
}

// prints out the current puzzle board
template<typename T>
void Puzzle<T>::print() {
	cout << "   ";
	// print column #s
	for(int i=1; i<=9; i++) {
		cout << i;	// print top column indices
		if((i%3)==0 && i!=9) cout<<"   ";	// gap between top column indices where inner puzzle borders are
		else cout<<" ";
	}
	cout << endl << "   ";			// gap before top row barrier
	for(int i=0; i<21; i++) cout<<"=";	// horizontal top row barrier
	cout << endl;

	// print row #s and the puzzle
	for(int i=0; i<9; i++) {
		cout << i+1 << "| ";	// row indices and vertical barrier
		for(int j=0; j<9; j++) {
			// print puzzle entry
			cout << puzMatrix[i][j];
			if((j%3)==2 && j!=8) cout<<" | ";	// print inner vertical borders
			else cout<<" ";
		}
		cout << endl;
		if((i%3)==2 && i!=8){
			cout << " : ";	// vertical barrier on border rows
			for(int k=0; k<21; k++){
				cout << "-";	// print inner horz. borders
			}
			cout << endl;
		}
	}// end for	
}

// prompts the user to solve a piece of the puzzle. returns the output of didWin()
template<typename T>
int Puzzle<T>::solvePiece() {
	// (row,column)
	int userRow;	// row
	int userCol;	// column
	T userVal;	// value the user wishes to place in (userRow,userCol)
	int errorPermVal=0;	// 1 if the coordinate chosen by the user is fixed on the original puzzle
	int isValid;	// 1 if the value is valid for the given coordinate. 2-4 otherwise
	do {
		// Prevent user from changing fixed initial puzzle positions
		if(errorPermVal==1) cout << "Error. Position entered is a fixed number. Please enter new coordinate...\n";
		// This will be displayed for first coordinate entered by user
		else cout << "You may now solve a piece. Enter your coordinate...\nReminder: Enter -1 at any time to quit.\n";
		errorPermVal = 1;	// causes error display for any invalid coordinate
		// User enters Row to change
		cout << "Row: ";
		cin >> userRow;
		// error checking: is position valid?
		while(userRow<1 || userRow>9) {
			// allow user to quit game is entered -1
			if(userRow==-1) {
				cout << "Thanks for playing!\n";
				return 0;
			}
			cout << "Error. Value entered is out of bounds.\nRow: ";
			cin >> userRow;
		}
		userRow--;	// adjust userRow to match index of puzzle output
		// User enters Col to change
		cout << "Column: ";
		cin >> userCol;
		// error checking: is position valid?
		while(userCol<1 || userCol>9) {
			// allow user to quit game is entered -1
			if(userCol==-1) {
				cout << "Thanks for playing!\n";
				return 0;
			}
			cout << "Error. Value entered is out of bounds.\nColumn: ";
			cin >> userCol;
		}
		userCol--;	// adjust userCol to match index of puzzle output
	// if the user tries to change a coordinate that was a 1-9 in the original skeleton, do not allow them to (b/c it was a fixed value in the original puzzle)
	} while(puzMatrix[userRow][userCol] != 0 && puzMatrix[userRow][userCol] == origSkeletonMatrix[userRow][userCol] );


	// at this point in solvePiece(), we know that the coordinates are within the bounds of the puzzle and not an original fixed value. Now the program will take in a value and see if the value is valid for the given coordinate
	// input a value 1-9
	cout << endl << "Enter the new value for (" << userRow+1 << "," << userCol+1 << "): ";
	cin >> userVal;
	// error checking: is the value 1-9?
	while(userVal<1 || userVal>9) {
		// allow user to quit game is entered -1
		if(userVal==-1) {
			cout << "Thanks for playing!\n";
			return 0;
		}
		cout << "Error. Value entered must between 1 and 9 (inclusive).";
		cout << endl << "Enter the new value for (" << userRow+1 << "," << userCol+1 << "): ";
		cin >> userVal;
	}

	// if the valid is entry, then change the value of (userRow,userCol), else tell the user where the conflicting value is
	isValid = validEntry(userRow, userCol, userVal);	// check if the value is valid
	system("clear");
	// if the value is valid, enter the value into the puzzle!
	if(isValid==1) {
		puzMatrix[userRow][userCol] = userVal;
	// else, tell the user why the value is invalid
	} else {
		cout << "Error. (" << userRow+1 << "," << userCol+1 << ") cannot be " << userVal << "\nA conflicting value exists in the ";
		switch(isValid){
			case 2:
				cout << "row";
				break;
			case 3:
				cout << "column";
				break;
			case 4:
				cout << "mini-grid";
				break;
			default:
				cout << "puzzle";
		}
		cout << endl << endl;
	}
	// returns 0 if the puzzle is complete. An integer greater than 0 otherwise
	return didWin();
}

// checks if the value for (puzRow,puzCol) is valid
template<typename T>
int Puzzle<T>::validEntry(int puzRow, int puzCol, T puzVal) {
	int isValid = 1;
	// each check makes sure that no matching values are found in the given row/col/grid (excluding its own position - ie. no error if user attempts to change a cell to its current value)
	// check row - error code = 2
	for(int j=0; j<9; j++) {
		if(puzMatrix[puzRow][j] == puzVal && !(j==puzCol) ) isValid=2;
	}
	// check column - error code = 3
	for(int i=0; i<9; i++) {
		if(puzMatrix[i][puzCol] == puzVal && !(i==puzRow) ) isValid=3;
	}
	// check mini grid - error code = 4
	// takes advantage of integer division
	int squareRow = puzRow/3;
	int squareCol = puzCol/3;
	for(int i=0+squareRow*3; i<=2+squareRow*3; i++) {
		for(int j=0+squareCol*3; j<=2+squareCol*3; j++) {
			if(puzMatrix[i][j] == puzVal && !(i==puzRow && j==puzCol) ) isValid=4;
		}
	}
	// returns 1 if the value is valid. 2-4 otherwise
	return isValid;
}

// checks if the user completed the puzzle. This function is quite simple because it takes advantage of the fact that the user is not allowed to change a 0 on the board unless the move is valid. Thus, if the last 0 is removed from the board, then every move must have been valid and the user has won!
template<typename T>
int Puzzle<T>::didWin() {
	int numZeros = 0;
	int size = puzMatrix.size();
	for(int i=0; i<size; i++)
		for(int j=0; j<size; j++)
			if(puzMatrix[i][j]==0)
				numZeros++;	// if this is ever incremented, then the player has not won
	// if the user has won, then make one final display before exiting the game
	if(numZeros==0) {
		cout<<"Congratulations! You win!\nHere is your finished puzzle...\n\n";
		print();
		cout << endl;
	}
	return numZeros;	// returns 0 if the user has successfully solved the puzzle
}
