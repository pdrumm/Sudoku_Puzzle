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
		void print();				// prints the current puzzle board
		int validEntry(int,int,T);		// returns 1 if the inputted coordinate and value is a valid solution for the puzzle, 0 otherwise
		int getSize(int,int);			// return the number of possible values for a specific puzzle cell
		T getVal(int,int,int);			// return a specific possible value for a specific puzzle cell
		void getPosVals();			// create the 3d vector of possible values
		int updatePosVals();			// update the possible value (3d) vector after an algorithm has been applied
		void singlePossibility();		// scanning algorithm
		void singleton();			// singleton algorithm
		void Patrick();				// personal algorithm
		int didWin();
	private:
		vector<vector<T> > solnMatrix;		// 2d vector stores the updated puzzle
		vector<vector<T> > origSkeletonMatrix;	// 2d vector stores the original puzzle
		vector<vector<vector<T> > > possibleVals;	// 3d vector stores the possible valid values for each cell in the puzzle
		ifstream inFile;	// object for reading from a file
};

#endif

// constructor opens file and stores the sudoku in a 2d vector
template<typename T>
Puzzle<T>::Puzzle(string puzzleFile) {
	inFile.open(puzzleFile.c_str(), ios::in);	// opens the puzzlefile for input
	T tempVar;					// temporary variable used to store puzzle values
	vector<T> tempVect;				// temporary vector stores each row one at a time
	for(int i=0; i<9; i++) {
		tempVect.clear();			// clear the previous row out
		for(int j=0; j<9; j++) {
			inFile >> tempVar;		// read in row i one variable at a time
			tempVect.push_back(tempVar);	// add the row variables to tempVect
		}
		// add row i to the puzzle
		solnMatrix.push_back(tempVect);
		// store original puzzle to the skeleton vector
		origSkeletonMatrix.push_back(tempVect);
	}
	inFile.close();					// close the puzzlefile used for input

}

// getPosVals() is called once - at the beginning of the program - to create the initial 3d vector of possible values for each puzzle cell. After that, values will gradually get erased using the updatePosVals() function
template<typename T>
void Puzzle<T>::getPosVals(){
	// Now fill the 3d vector with all possible valid values for the given puzzle
	vector<T> tempVect;			// temporary 1d vector
	vector<vector<T> > temp2dVect;		// temporary 2d vector
	possibleVals.clear();
	// Manuever through ROWS
	for(int i=0; i<9; i++) {
		temp2dVect.clear();		// clear the temp 2d vect
		// Manuever through COLUMNS
		for(int j=0; j<9; j++) {
			tempVect.clear();	// clear the temp 1d vect
			// Manuever through possible puzzle values 1-9
			for(T k=1; k<=9; k++) {
				// if the possible puzzle value is valid, then add it to the possible value vector
				if(validEntry(i,j,k)==1){
					// add k to the possible values for cell (i,j)
					tempVect.push_back(k);
				}
			}
			// adds vector of possible values to corresponding cell
			temp2dVect.push_back(tempVect);
		}
		// add entire row of temporary vectors to the 3d vector
		possibleVals.push_back(temp2dVect);
	}
}

// updatePosVals() is used to check all possible values in the 3d vector, and then decide whether each of those values is still a valid possible entry (due to the changes applied to the solution board by the other algorithms). If a possible value in the 3d vector is no longer possible, it will erase the value from the 3d vector.
template<typename T>
int Puzzle<T>::updatePosVals() {
	int posValSize;				// temporary variable used to store the number of possible values for each puzzle cell
	int changeMade = 0;			// boolean variable used to see if a possible value is erased. If no value is erased, then changeMade=0 and the 3d vector was unaffected by this function
	for(int i=0; i<9; i++) {		// row
		for(int j=0; j<9; j++) {	// column
			for(T k=1; k<=9; k++) {	// puzzle values 1-9
				// if the puzzle value is not valid for this puzzle cell, check to see if it is listed as a possible value
				if(validEntry(i,j,k)==0){
					posValSize=getSize(i,j);
					for(int m=0; m<posValSize; m++) {	// traverse possible values of cell (i,j)
						if(possibleVals[i][j][m]==k) {// this is true if a previously possible value is no longer valid for the given cell
							possibleVals[i][j].erase(possibleVals[i][j].begin() + m);	// erase the invalid possibility
							m=posValSize;	// this will exit the innermost for loop
							changeMade=1;	// this function did indeed make a change to the 3d vector
						}
					}
				}
			}
		}
	}
	return changeMade;	// returns 1 if a change was made to the 3d vect. 0 otherwise
}

// singlePossibility() implements the scanning algorithm. That is, if any puzzle cell has only one possible puzzle value, then this funtion sets the corresponding cell in the solution matrix equal to that value
template<typename T>
void Puzzle<T>::singlePossibility() {
	for(int i=0; i<9; i++) {		// row
		for(int j=0; j<9; j++) {	// column
			// if the cell has only one possible value, and it has not already been placed in the solution matrix, then assign the solution matrix that value
			if(getSize(i,j)==1 && solnMatrix[i][j]==0)
				solnMatrix[i][j]=possibleVals[i][j][0];
		}
	}
}

// singleton() implements the singleton algorithm. This algorithm checks each row, column, and minigrid in the puzzle. If there exists only one cell of the nine that has a certain possible value 1-9, then this function sets the corresponding location of that one cell in the solution matrix equal to that value
template<typename T>
void Puzzle<T>::singleton() {
	vector<T> posValCount;			// counts the number of cells in a given row/col/grid that has each puzzle value 1-9. For example, if there are 3 cells in a row that could potentially be a 2, then posValCount[2-1]=posValCount[1]=3
	vector<T> posValLocation;		// keeps track of the location of the most recent cell for each value incremented in posValCount. For example, if while checking a row, a 5 is found to be a possible value in the 3rd column, then posValLocation[5-1]=posValLocation[4]= 3-1 = 2
	vector<vector<T> > posValLocation2;	// performs the same functionality for the minigrid as the 1d vector does for the row and column. However this vector must store both the row and column because neither is constant throughout a minigrid
	vector<T> zeroVect;			// vector of zeros of size 9
	vector<vector<T> > zeroVect2;		// vector of zeros of size 2x9
	for(int i=0; i<9; i++) {
		zeroVect.push_back(0);		// fill with zeros
	}
	zeroVect2.push_back(zeroVect);		// fill with zeros
	zeroVect2.push_back(zeroVect);
	// check row
	for(int i=0; i<9; i++) {		// row
		posValCount = zeroVect;
		posValLocation = zeroVect;
		// go through all possible values for each cell in the given row
		for(int j=0; j<9; j++) {	// column
			for(int k=0; k<getSize(i,j); k++) {
				posValCount[getVal(i,j,k)-1]++;
				posValLocation[getVal(i,j,k)-1]=j;
			}
		}
		for(int j2=0; j2<9; j2++) {
			// if the count == 1, then that value must be the solution to the corresponding cell in the location vector
			if(posValCount[j2]==1) {
				solnMatrix[i][posValLocation[j2]]=j2+1;
			}
		}
	}
	// check column
	for(int j=0; j<9; j++) {		// column
		posValCount = zeroVect;
		posValLocation = zeroVect;
		// go through all possible values for each cell in the given column
		for(int i=0; i<9; i++) {	// row
			for(int k=0; k<getSize(i,j); k++) {
				posValCount[getVal(i,j,k)-1]++;
				posValLocation[getVal(i,j,k)-1]=i;
			}
		}
		for(int i2=0; i2<9; i2++) {
			// if the count == 1, then that value must be the solution to the corresponding cell in the location vector
			if(posValCount[i2]==1) {
				solnMatrix[posValLocation[i2]][j]=i2+1;
			}
		}
	}
	// check mini grid
	for(int squareRow=0; squareRow<=2; squareRow++) {
	for(int squareCol=0; squareCol<=2; squareCol++) {
		posValCount = zeroVect;
		posValLocation2 = zeroVect2;
		// go through all possible values for each cell in the given minigrid
		for(int i=0+squareRow*3; i<=2+squareRow*3; i++) {		// row
			for(int j=0+squareCol*3; j<=2+squareCol*3; j++) {	// column
				for(int k=0; k<getSize(i,j); k++) {
					posValCount[getVal(i,j,k)-1]++;
					posValLocation2[0][getVal(i,j,k)-1]=i;
					posValLocation2[1][getVal(i,j,k)-1]=j;
				}
			}
		}
		for(int i2=0; i2<9; i2++) {
			// if the count == 1, then that value must be the solution to the corresponding cell in the location vector
			if(posValCount[i2]==1) {
				solnMatrix[posValLocation2[0][i2]][posValLocation2[1][i2]]=i2+1;
			}
		}
	}
	}
}

// Patrick() is an algorithm that I created. The algorithm checks each row and column to see if there are n equal possible value vectors of size n. If so, then no other cells in that row or column can have any values in those possible value vectors.
// For example, if there are two cells in the same row with the possible value vector of <2,3>, then no other cells in that row can have 2 or 3 in their possible value vector
template<typename T>
void Puzzle<T>::Patrick() {
	int matchCount;
	vector<T> matchLocation;
	// check row
	for(int i=0; i<9; i++) {		// row
		for(int j=0; j<8; j++) {	// column
			matchCount = 0;
			matchLocation.clear();
			for(int k=j+1; k<9; k++) {	// for all cells in the row to the right of the cell being checked...
				if(possibleVals[i][j]==possibleVals[i][k]){	// if the possible value vectors are equal...
					matchCount++;				// add one to the match count
					matchLocation.push_back(k);		// and store the location of the match
				}
			}
			// if the number of equivalent vectors is equal to the size of the vectors...
			if(matchCount+1 == getSize(i,j) ) {
				// erase -from all cells that were not found to be exact matches- every value that is in the matching possible value vectors
				for(int j2=0; j2<9; j2++) {
					for(int k2=0; k2<getSize(i,j2); k2++) {
						for(int m2=0; m2<getSize(i,j); m2++) {
							if(getVal(i,j,m2)==getVal(i,j2,k2) && j2!=j) {	// if a possible value being erased is found, and it is not in the original cell that matched to equivalent vectors...
								int matchAccounted=0;
								for(int n=0; n<matchLocation.size(); n++) {
									if(matchLocation[n]==j2) {	// if the cell was stored in the location vector, do not erase the possible values
										matchAccounted=1;
									}
								}
								if(matchAccounted==0){
									possibleVals[i][j2].erase(possibleVals[i][j2].begin() + k2);
								}
							}
						}
					}
				}
			}
		}
	}
	// check column
	for(int j=0; j<9; j++) {		// column
		for(int i=0; i<8; i++) {	// row
			matchCount = 0;
			matchLocation.clear();
			for(int k=i+1; k<9; k++) {	// for all cells in the row below the cell being checked...
				if(possibleVals[i][j]==possibleVals[k][j]){	// if the possible value vectors are equal...
					matchCount++;				// add one to the match count
					matchLocation.push_back(k);		// and store the location of the match
				}
			}
			// if the number of equivalent vectors is equal to the size of the vectors...
			if(matchCount+1 == getSize(i,j) ) {
				// erase -from all cells that were not found to be exact matches- every value that is in the matching possible value vectors
				for(int i2=0; i2<9; i2++) {
					for(int k2=0; k2<getSize(i2,j); k2++) {
						for(int m2=0; m2<getSize(i,j); m2++) {
							if(getVal(i,j,m2)==getVal(i2,j,k2) && i2!=i) {	// if a possible value being erased is found, and it is not in the original cell that matched to equivalent vectors...
								int matchAccounted=0;
								for(int n=0; n<matchLocation.size(); n++) {
									if(matchLocation[n]==i2) {	// if the cell was stored in the location vector, do not erase the possible values
										matchAccounted=1;
									}
								}
								if(matchAccounted==0){
									possibleVals[i2][j].erase(possibleVals[i2][j].begin() + k2);
								}
							}
						}
					}
				}
			}
		}
	}
}

// return the number of possible values in cell (i,j)
template<typename T>
int Puzzle<T>::getSize(int i,int j){
	return possibleVals[i][j].size();
}

// return the possible value k, in cell (i,j)
template<typename T>
T Puzzle<T>::getVal(int i,int j,int k){
	return possibleVals[i][j][k];
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
			cout << solnMatrix[i][j];
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

// checks if the value for (puzRow,puzCol) is valid
template<typename T>
int Puzzle<T>::validEntry(int puzRow, int puzCol, T puzVal) {
	int isValid = 1;
	// If the solution matrix does not have zero stored for this cell, then it has already been solved. So if puzVal equals the value in the solution matrix, it is valid. Else, it is invalid
	if(solnMatrix[puzRow][puzCol]!=0) {
		if(puzVal==solnMatrix[puzRow][puzCol]) isValid=1;
		else isValid=0;
	}else{
		// each check makes sure that no matching values are found in the given row/col/grid (excluding its own position - ie. no error if user attempts to change a cell to its current value)
		// check row
		for(int j=0; j<9; j++) {
			if(solnMatrix[puzRow][j] == puzVal && !(j==puzCol) ) isValid=0;
		}
		// check column
		for(int i=0; i<9; i++) {
			if(solnMatrix[i][puzCol] == puzVal && !(i==puzRow) ) isValid=0;
		}
		// check mini grid
		// takes advantage of integer division
		int squareRow = puzRow/3;
		int squareCol = puzCol/3;
		for(int i=0+squareRow*3; i<=2+squareRow*3; i++) {
			for(int j=0+squareCol*3; j<=2+squareCol*3; j++) {
				if(solnMatrix[i][j] == puzVal && !(i==puzRow && j==puzCol) ) isValid=0;
			}
		}
	}
	// returns 1 if the value is valid. 0 otherwise
	return isValid;
}

// checks if the puzzle has been solved. This function takes advantage of the fact that the program will not change a 0 on the board unless the move is valid. Thus, if the last 0 is removed from the board, then every move must have been valid and the puzzle has been solved
template<typename T>
int Puzzle<T>::didWin() {
	int numZeros = 0;
	int size = solnMatrix.size();
	for(int i=0; i<size; i++)
		for(int j=0; j<size; j++)
			if(solnMatrix[i][j]==0)
				numZeros++;	// if this is ever incremented, then the player has not won
	return numZeros;	// returns 0 if the puzzle has successfully been solved
}
