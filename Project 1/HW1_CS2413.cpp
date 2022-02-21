#include <iostream>
using namespace std;

class CRM {
protected:
	int n; //The number rows of the original matrix
	int m; //The number of columns of the original matrix
	int nonZeros; //The number of non-zero elements in the original matrix
	int valIndex; //The index that the next value will be added
	int rowIndex; //The index that the next row will be added
	int rowValue; //The index to be put in the rowPos array
	int colIndex; //The index that the next column will be added
	int* values; //Array that contains all non-zero values of the matrix, assuming they are all integers
	int* rowPos; //Array that for each row of the original matrix contains the position in the values matrix in which the first non-zero element of this row is stored.
	int* givenRows; //Stores all the rows given by the user
	int* colPos; //Array that contains the column number for each one of the non-zero values in the original matrix.
	//There may be others you may need
public:
	CRM(); //default or empty constructor
	CRM(int rows, int cols, int numNonZeros);
	int getNumRows();
	void addValue(int value); //add a new value in the values array
	void addRow(int row); //add a row number in the rowPos array
	void addColumn(int col);//add a column number in the colPos array
	void display(); //print the contents of the three arrays. Each array must be on a different line (separated by a 
					//new line character) and you have exactly a single space between each value printed.
	int mostInfluentialUser(); //find the most influential user
	int mostActiveUser(); //find the most active user
	int* influentialUsers(); //compute vector of users ranked by how much influential they are
	int* activeUsers(); //compute vector of users ranked by how much active they are
	int* sort(int* array, int* numArray, int size); //sort the given array in descending order
	~CRM(); //destructor
	//You may define and use any other method you need		
};
//some of the methods – Katia Papakonstantinopoulou
CRM::CRM() {
	n = 0;
	m = 0;
	nonZeros = 0;
	valIndex = 0;
	rowIndex = 0;
	rowValue = 0;
	colIndex = 0;
	values = NULL;
	rowPos = NULL;
	givenRows = NULL;
	colPos = NULL;
}
CRM::CRM(int rows, int cols, int numNonZeros) {
	n = rows;
	m = cols;
	valIndex = 0;
	rowIndex = 0;
	rowValue = 0;
	colIndex = 0;
	nonZeros = numNonZeros;
	values = new int[nonZeros];
	rowPos = new int[n];
	givenRows = new int[n];
	colPos = new int[nonZeros];

	for (int i = 0; i < n; i++)
	{
		rowPos[i] = -1;
		givenRows[i] = -1;
	}
}

int CRM::getNumRows()
{
	return n; 
}

void CRM::addValue(int value)
{
	values[valIndex++] = value;
}

void CRM::addRow(int row)
{	
	// Auto sets the first index
	if (rowIndex < 1)
	{
		rowPos[rowIndex] = rowValue;
		givenRows[rowIndex] = row;
		rowIndex++;
		rowValue++;
		return;
	}

	int previous = rowIndex - 1;
	
	// Checks the previous row and see if it is equal to the current inputted row
	if (givenRows[previous] != row)
	{
		// If row - givenRows[previous > 1] then it means that one of the indexes will be -1 so we do rowIndex++
		if (row - givenRows[previous] > 1)
			rowIndex++;

		rowPos[rowIndex] = rowValue;
		givenRows[rowIndex] = row;
		rowIndex++;
	}

	rowValue++;
}

void CRM::addColumn(int col)
{
	colPos[colIndex++] = col;
}

void CRM::display()
{
	cout << "values: ";

	for (int i = 0; i < nonZeros; i++)
	{
		cout << values[i] << " ";
	}

	cout << "\nrowPos: ";

	for (int i = 0; i < n; i++)
	{
		cout << rowPos[i] << " ";
	}

	cout << "\ncolPos: ";

	for (int i = 0; i < nonZeros; i++)
	{
		cout << colPos[i] << " ";
	}

	cout << endl << endl;
}

int CRM::mostInfluentialUser() {
	return influentialUsers()[0];
}

int* CRM::influentialUsers() {
	int* outputVector = new int[m];
	int* summedValues = new int[m];

	// Sets all the values in the array to 0
	for (int i = 0; i < n; i++)
	{
		outputVector[i] = 0;
		summedValues[i] = 0;
	}

	// Cycles through all the columns
	for (int column = 0; column < m; column++)
	{
		int retweets = 0;

		// Cycles through all the values in the values array
		for (int valArrIndex = 0; valArrIndex < nonZeros; valArrIndex++)
		{
			// If the columns match, then it adds the value in the same index in the values array to retweets
			if (colPos[valArrIndex] == column)
				retweets += values[valArrIndex];
		}

		outputVector[column] = column;
		summedValues[column] = retweets;
	}

	sort(summedValues, outputVector, m);

	return outputVector;
}

int CRM::mostActiveUser()
{
	return activeUsers()[0];
}

int* CRM::activeUsers()
{
	int* outPutVector = new int[n];
	int* summedValues = new int[n];
	int index = 0;

	for (int i = 0; i < n; i++)
	{
		outPutVector[i] = 0;
		summedValues[i] = 0;
	}

	// Goes through all the rows
	for (int row = 0; row < n; row++)
	{
		// If rowPos at row is -1. then we set outPutVector at row to row and continues with the for loop
		if (rowPos[row] == -1)
		{
			outPutVector[row] = row;
			continue;
		}

		int tweets = 0;
		int end = nonZeros;

		// Checks to see if rowPos at row + 1 is -1
		for (int i = row + 1; i < n; i++)
		{
			// If it is not -1, set end to it and break the for loop
			if (rowPos[i] != -1)
			{
				end = rowPos[i];
				break;
			}
		}

		// Adds up all the tweets
		for (int i = rowPos[row]; i < end; i++)
		{
			tweets += values[i];
		}

		outPutVector[row] = row;
		summedValues[row] = tweets;
		sort(summedValues, outPutVector, n);
	}

	return outPutVector;
}

int* CRM::sort(int* array, int* numArray, int size)
{
	// loop to access each array element
	for (int x = 0; x < size; x++) 
	{

		// loop to compare array elements
		for (int i = 0; i < size - x; i++) 
		{
			// compare two adjacent elements
			if (array[i] < array[i + 1]) 
			{

				// swapping elements
				int temp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = temp;

				int temp2 = numArray[i + 1];
				numArray[i + 1] = numArray[i];
				numArray[i] = temp2;
			}
		}
	}

	return numArray;
}

CRM::~CRM() {
	if (values != NULL) delete[] values;
	if (rowPos != NULL) delete[] rowPos;
	if (colPos != NULL) delete[] colPos;
	cout << "CRM Object Destroyed!!" << endl;
	n = 0;
	m = 0;
	nonZeros = 0;
}

int main() {

	CRM* A;
	int numRows, numColumns, numNonZeros;
	int row, col, value;

	//read in the first matrix
	cin >> numRows >> numColumns;
	cin >> numNonZeros;

	A = new CRM(numRows, numColumns, numNonZeros);
	for (int i = 0; i < numNonZeros; i++) {
		cin >> row >> col >> value;
		(*A).addValue(value);
		(*A).addRow(row);//needs to be done cleverly in the method
		(*A).addColumn(col);
	}
	(*A).display();

	//Find most influential user
	int mostInf = (*A).mostInfluentialUser();
	cout << "Most influential user: " << mostInf << endl;
	cout << endl;

	//Find most active user
	int mostAct = (*A).mostActiveUser();
	cout << "Most active user: " << mostAct << endl;
	cout << endl;

	//Rank users based on how much influential they are
	int* influentialityVector = (*A).influentialUsers();
	cout << "Users ranked by Influentiality: " << endl;
	for (int i = 0; i < (*A).getNumRows(); i++)
		cout << influentialityVector[i] << " ";
	cout << endl << endl;

	//Rank users based on how much active they are
	int* activityVector = (*A).activeUsers();
	cout << "Users ranked by Activity: " << endl;
	for (int i = 0; i < (*A).getNumRows(); i++)
		cout << activityVector[i] << " ";
	cout << endl;

	// Call the destructors
	delete A;
	delete[] influentialityVector;
	delete[] activityVector;

	return 0;
}
