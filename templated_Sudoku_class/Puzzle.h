/*
 * Patrick Drumm
 * lab 5, cse20212
 */

#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

template<typename T>
class Puzzle {
	public:
		Puzzle(string="sampleSodoku.txt");	// constructor
		void print();	// displays the puzzle matrix
	private:
		vector<vector<T> > puzMatrix;	// stores the puzzl
		ifstream inFile;	// object for reading from a file
};

#endif

// nondefault constructor takes in the filename that holds the puzzle as an input
template<typename T>
Puzzle<T>::Puzzle(string puzzleFile) {
	inFile.open(puzzleFile.c_str(), ios::in);	// open the file
	T tempVar;	// temporary variable used to store each value of the puzzle
	vector<T> tempVect;	// temporary vectory stores each row of the puzzle
	for(int i=0; i<9; i++) {
		tempVect.clear();	// clear the row vector
		for(int j=0; j<9; j++) {
			inFile >> tempVar;	// store the next puzzle value
			tempVect.push_back(tempVar);	// add the values of row i
		}
		puzMatrix.push_back(tempVect);	// add row i to the puzzle matrix
	}
	inFile.close();	// close the file
}

// display the puzzle matrix
template<typename T>
void Puzzle<T>::print() {
	for(int i=0; i<9; i++) {
		for(int j=0; j<9; j++) {
			// print each value of the puzzle followed by a space
			cout << puzMatrix[i][j] << " ";
		}
	cout << endl;	// separate rows by a newline
	}	
}
