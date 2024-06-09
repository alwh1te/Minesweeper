//
// Created by Alexander on 07.06.2024.
//

#ifndef MINESWEEPER_MINESWEEPER_H
#define MINESWEEPER_MINESWEEPER_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVector>
#include <QMouseEvent>

class Minesweeper : public QWidget {
    Q_OBJECT

public:
    explicit Minesweeper(QWidget *parent = nullptr);
    ~Minesweeper();

private:
    int boardSize;
    int numMines;
    int buttonHeight; // Height of each button
    int buttonWidth; // Width of each button

    QVector<QVector<QPushButton*>> buttons; // Grid of buttons representing the game board
    QVector<QVector<int>> mines; // Grid indicating the presence of mines
    QVector<QVector<bool>> revealed; // Grid indicating whether a cell is revealed
    QVector<QVector<bool>> flagged; // Grid indicating whether a cell is flagged

    QGridLayout *gridLayout; // Layout for arranging buttons
    bool debugMode; // Flag indicating debug mode

    int row; // Store the row of the last clicked cell
    int col; // Store the column of the last clicked cell

    void setupBoard(); // Setup the game board
    void placeMines(int rowClicked, int colClicked); // Place mines on the board after the first click
    void revealCell(int row, int col); // Reveal cell and its neighbors
    void checkWin(); // Check if the player has won the game
    void gameOver(bool win); // Handle game over (win or lose)
    void saveGame(); // Save current game state to .ini file
    void loadGame(); // Load game state from .ini file

protected:
    void mousePressEvent(QMouseEvent *event) override; // Handle mouse press event

private slots:
    void newGame(); // Start a new game
    void cellClicked(); // Handle cell click
};

#endif//MINESWEEPER_MINESWEEPER_H
