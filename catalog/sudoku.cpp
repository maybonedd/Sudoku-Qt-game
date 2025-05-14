#include "sudoku.h"
#include <ctime>

using namespace std;

Sudoku::Sudoku(int Number_columns, int Number_missing_digits)
{
    srand(time(0));
    this->Number_columns = Number_columns;
    this->Number_missing_digits = Number_missing_digits;


    double SRNd = sqrt(Number_columns);
    Square_root_N = (int)SRNd;
    mat = new int*[Number_columns];
    valid_mat = new int*[Number_columns];

    for (int i = 0; i < Number_columns; i++)
    {
        mat[i] = new int[Number_columns];
        valid_mat[i] = new int[Number_columns];

        memset(mat[i], 0, Number_columns * sizeof(int));
        memset(valid_mat[i], 0, Number_columns * sizeof (int));
    }
}


void Sudoku::fillValues()
{
    fillDiagonal();
    fillRemaining(0, Square_root_N);
    setValidMat();
    removeMissingDigits();
}


int Sudoku::getMatVal(int r, int c)
{
    return mat[r][c];
}


int Sudoku::getValidMatVal(int r, int c)
{
    return valid_mat[r][c];
}


void Sudoku::fillDiagonal()
{
    for (int i = 0; i < Number_columns; i = i + Square_root_N)
    {
        fillBox(i, i);
    }
}


bool Sudoku::unUsedInBox(int rowStart, int colStart, int num)
{
    for (int i = 0; i < Square_root_N; i++) {
        for (int j = 0; j < Square_root_N; j++) {
            if (mat[rowStart + i][colStart + j]
                    == num) {
                return false;
            }
        }
    }
    return true;
}


void Sudoku::fillBox(int row, int col)
{
    int num;
    for (int i = 0; i < Square_root_N; i++) {
        for (int j = 0; j < Square_root_N; j++) {
            do {
                num = randomGenerator(Number_columns);
            } while (!unUsedInBox(row, col, num));
            mat[row + i][col + j] = num;
        }
    }
}


int Sudoku::randomGenerator(int num)
{
    return (int)floor(
                (float)(rand() / double(RAND_MAX) * num + 1));
}


bool Sudoku::unUsedInRow(int i, int num)
{
    for (int j = 0; j < Number_columns; j++) {
        if (mat[i][j] == num) {
            return false;
        }
    }
    return true;
}


bool Sudoku::fillRemaining(int i, int j)
{
    if (j >= Number_columns && i < Number_columns - 1) {
        i = i + 1;
        j = 0;
    }
    if (i >= Number_columns && j >= Number_columns) {
        return true;
    }
    if (i < Square_root_N) {
        if (j < Square_root_N) {
            j = Square_root_N;
        }
    }
    else if (i < Number_columns - Square_root_N) {
        if (j == (int)(i / Square_root_N) * Square_root_N) {
            j = j + Square_root_N;
        }
    }
    else {
        if (j == Number_columns - Square_root_N) {
            i = i + 1;
            j = 0;
            if (i >= Number_columns) {
                return true;
            }
        }
    }
    for (int num = 1; num <= Number_columns; num++) {
        if (CheckIfSafe(i, j, num)) {
            mat[i][j] = num;
            if (fillRemaining(i, j + 1)) {
                return true;
            }
            mat[i][j] = 0;
        }
    }
    return false;
}


void Sudoku::removeMissingDigits()
{
    int count = Number_missing_digits;
    while (count != 0) {
        int cellId = randomGenerator(Number_columns * Number_columns) - 1;
        int i = (cellId / Number_columns);
        int j = cellId % 9;
        if (j != 0) {
            j = j - 1;
        }

        if (mat[i][j] != 0) {
            count--;
            mat[i][j] = 0;
        }
    }
}


bool Sudoku::CheckIfSafe(int i, int j, int num)
{
    return (
                unUsedInRow(i, num) && unUsedInCol(j, num)
                && unUsedInBox(i - i % Square_root_N, j - j % Square_root_N, num));
}


bool Sudoku::unUsedInCol(int j, int num)
{
    for (int i = 0; i < Number_columns; i++) {
        if (mat[i][j] == num) {
            return false;
        }
    }
    return true;
}


void Sudoku::printSudoku()
{
    for (int i = 0; i < Number_columns; i++) {
        for (int j = 0; j < Number_columns; j++) {
            cout << to_string(mat[i][j]) + " ";
        }
        cout << endl;
    }
    cout << endl;
}


void Sudoku::setValidMat()
{
    for (int i = 0; i < Number_columns; i++)
    {
        for (int j = 0; j < Number_columns; j++)
        {
            valid_mat[i][j] = mat[i][j];
        }
    }
}

bool Sudoku::solve() {
    int row, col;

    if (!findEmptyLocation(row, col)) {
        return true;
    }

    for (int num = 1; num <= Number_columns; num++) {
        if (CheckIfSafe(row, col, num)) {
            mat[row][col] = num;

            if (solve()) {
                return true;
            }

            mat[row][col] = 0;
        }
    }

    return false;
}

bool Sudoku::findEmptyLocation(int &row, int &col) {
    for (row = 0; row < Number_columns; row++) {
        for (col = 0; col < Number_columns; col++) {
            if (mat[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}
