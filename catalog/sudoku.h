#ifndef SUDOKU_H
#define SUDOKU_H

#include <bits/stdc++.h>


class Sudoku
{
public:
    Sudoku(int N, int K);
    void fillValues();

    int getMatVal(int r, int c);
    int getValidMatVal(int r, int c);

    void printSudoku();

private:
    int** mat;
    int** valid_mat;
    int Number_columns;
    int Square_root_N;
    int Number_missing_digits;

    void fillDiagonal();
    bool unUsedInBox(int rowStart, int colStart, int num);
    void fillBox(int row, int col);
    int randomGenerator(int num); // функция для генерации числа
    bool CheckIfSafe(int i, int j, int num);
    bool unUsedInRow(int i, int num);
    bool unUsedInCol(int j, int num);
    bool fillRemaining(int i, int j);
    void removeMissingDigits();

    void setValidMat();
};

#endif // SUDOKU_H
