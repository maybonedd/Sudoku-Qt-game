#include "board.h"
#include "utilities.h"

Board::Board()
    : m_clickedSquare(nullptr), m_grid(BOARD_STYLE::grid_size, QVector<int>(BOARD_STYLE::grid_size))
{
    m_sudoku = new Sudoku(BOARD_STYLE::grid_size, 25);
    m_sudoku->fillValues();
    for (int i = 0; i < m_grid.size(); i++)
    {
        for (int j = 0; j < m_grid[i].size(); j++)
        {
            m_grid[i][j] = m_sudoku->getMatVal(i, j);
        }
    }
    createSquares();
}


void Board::removeNumbers()
{
    delete m_sudoku;
}


void Board::handleInput(int num)
{
    if(m_clickedSquare)
    {
        m_clickedSquare->setEnteredText(num);
        m_grid[m_choosenSquare.x()][m_choosenSquare.y()] = num;
    }
}


void Board::generate()
{
    delete m_sudoku;
    m_sudoku = new Sudoku(BOARD_STYLE::grid_size, 25);
    m_sudoku->fillValues();

    for (int i = 0; i < m_grid.size(); i++)
    {
        for (int j = 0; j < m_grid[i].size(); j++)
        {
            m_grid[i][j] = m_sudoku->getMatVal(i, j);
        }
    }


    for (int i = 0; i < m_grid.size(); i++)
    {
        for (int j = 0; j < m_grid[i].size(); j++)
        {
            m_squares[i][j]->setText(m_grid[i][j]);
            m_squares[i][j]->m_locked = false;
            if(m_grid[i][j] != 0)
            {
                m_squares[i][j]->m_locked = true;
            }
            m_squares[i][j]->m_correctNumber = m_sudoku->getValidMatVal(i, j);
            m_squares[i][j]->setDefaultColor();
            m_squares[i][j]->setBlackTextColor();
            m_squares[i][j]->m_enteredNumber = 0;
        }
    }
}

QVector<QVector<Square *> > Board::squares() const
{
    return m_squares;
}


void Board::handleClicked(QPointF clickedPoint)
{
    m_clickedSquare = nullptr;
    unselectSquares();
    int i = 0;
    int j = 0;
    foreach(QVector<Square*> vec, squares())
    {
        foreach(Square* square, vec)
        {
            if(square->contains(clickedPoint-square->pos()) && !square->m_locked)
            {
                m_clickedSquare = square;
                m_clickedSquare->setActiveColor();
                m_choosenSquare = QPoint(i,j%BOARD_STYLE::grid_size);
            }
            j++;
        }
        i++;
    }
}


void Board::doSolution() {
    if (m_gameMode == GameMode::USER_CREATE) {
        Sudoku tempSudoku(BOARD_STYLE::grid_size, 0);
        for (int i = 0; i < BOARD_STYLE::grid_size; i++) {
            for (int j = 0; j < BOARD_STYLE::grid_size; j++) {
                tempSudoku.setMatVal(i, j, m_grid[i][j]);
            }
        }

        if (tempSudoku.solve()) {
            for (int i = 0; i < BOARD_STYLE::grid_size; i++) {
                for (int j = 0; j < BOARD_STYLE::grid_size; j++) {
                    m_grid[i][j] = tempSudoku.getMatVal(i, j);
                    if (!m_squares[i][j]->m_locked) {
                        m_squares[i][j]->setEnteredText(m_grid[i][j]);
                    }
                }
            }
        } else {
            qDebug() << "This puzzle has no solution!";
        }
    } else {
        for (int i = 0; i < BOARD_STYLE::grid_size; ++i) {
            for (int j = 0; j < BOARD_STYLE::grid_size; ++j) {
                m_grid[i][j] = m_sudoku->getValidMatVal(i, j);
                if (!m_squares[i][j]->m_locked) {
                    m_squares[i][j]->setEnteredText(m_grid[i][j]);
                }
            }
        }
    }
}


void Board::createSquares()
{
    m_squares.resize(BOARD_STYLE::grid_size);
    for (int i = 0; i < m_grid.size(); i++)
    {
        for (int j = 0; j < m_grid[i].size(); j++)
        {
            Square *s = new Square(QPointF(BOARD_STYLE::offset + BOARD_STYLE::square_pixel_size*i,
                                                                  BOARD_STYLE::offset + BOARD_STYLE::square_pixel_size*j), m_grid[i][j]);
            s->setText(m_grid[i][j]);
            s->m_locked = false;
            if(m_grid[i][j] != 0)
            {
                s->m_locked = true;
            }
            s->m_correctNumber = m_sudoku->getValidMatVal(i, j);
            m_squares[i].push_back(s);
        }
    }
}


void Board::unselectSquares()
{
    foreach(QVector<Square*> vec, squares())
    {
        foreach(Square* square, vec)
        {
            square->setDefaultColor();
        }
    }
}

void Board::switchMode() {
    if (m_gameMode == GameMode::NORMAL) {
        setGameMode(GameMode::USER_CREATE);
    } else {
        setGameMode(GameMode::NORMAL);
    }
}

void Board::setGameMode(GameMode mode) {
    m_gameMode = mode;
    if (mode == GameMode::USER_CREATE) {
        clearBoard();
    } else {
        generate();
    }
}

void Board::clearBoard() {
    for (int i = 0; i < BOARD_STYLE::grid_size; i++) {
        for (int j = 0; j < BOARD_STYLE::grid_size; j++) {
            m_grid[i][j] = 0;
            m_squares[i][j]->setText(0);
            m_squares[i][j]->m_locked = false;
            m_squares[i][j]->m_correctNumber = 0;
            m_squares[i][j]->setDefaultColor();
            m_squares[i][j]->setBlackTextColor();
        }
    }
    m_moveHistory.clear();
}

void Board::undoLastMove() {
    if (!m_moveHistory.isEmpty()) {
        QPoint lastMove = m_moveHistory.pop();
        int row = lastMove.x();
        int col = lastMove.y();

        m_grid[row][col] = 0;
        m_squares[row][col]->setText(0);
        m_squares[row][col]->m_enteredNumber = 0;
        m_squares[row][col]->setDefaultColor();
    }
}

bool Board::validateUserInput(int row, int col, int num) const {
    if (num == 0) return true;

    for (int j = 0; j < BOARD_STYLE::grid_size; j++) {
        if (j != col && m_grid[row][j] == num) {
            return false;
        }
    }

    for (int i = 0; i < BOARD_STYLE::grid_size; i++) {
        if (i != row && m_grid[i][col] == num) {
            return false;
        }
    }

    int boxRow = row - row % 3;
    int boxCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if ((boxRow + i != row || boxCol + j != col) &&
                m_grid[boxRow + i][boxCol + j] == num) {
                return false;
            }
        }
    }

    return true;
}

void Board::handleInput(int num) {
    if (m_clickedSquare) {
        if (m_gameMode == GameMode::USER_CREATE) {
            if (validateUserInput(m_choosenSquare.x(), m_choosenSquare.y(), num)) {
                m_grid[m_choosenSquare.x()][m_choosenSquare.y()] = num;
                m_clickedSquare->setEnteredText(num);
                m_moveHistory.push(m_choosenSquare);
            } else {
                m_clickedSquare->setText(0);
                m_clickedSquare->m_enteredNumber = 0;
            }
        } else {
            m_clickedSquare->setEnteredText(num);
            m_grid[m_choosenSquare.x()][m_choosenSquare.y()] = num;
        }
    }
}

