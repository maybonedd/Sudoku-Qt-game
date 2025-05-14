#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include "sudoku.h"

#include <QVector>
#include <QQueue>
#include <QPair>


class Board
{
public:
    Board();
    ~Board(){};
    void removeNumbers();
    void updateSquares();
    void handleInput(int num);
    void generate();

    QVector<QVector<Square*>> squares() const;

    void handleClicked(QPointF clickedPoint);
    void doSolution();

    void switchMode();
    void clearBoard();
    void undoLastMove();
    bool validateUserInput(int row, int col, int num) const;
    void setGameMode(GameMode mode);
    GameMode gameMode() const;

private:
    void createSquares();
    void unselectSquares();

    Sudoku* m_sudoku;
    Square* m_clickedSquare;
    QVector<QVector<int> > m_grid;
    QVector<QVector<Square*>> m_squares;
    QPoint m_choosenSquare;
    GameMode m_gameMode;
    QVector<QVector<int>> m_userGrid;
    QStack<QPoint> m_moveHistory;
};

#endif // BOARD_H
