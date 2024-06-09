#include "Minesweeper.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTextStream>


Minesweeper::Minesweeper(QWidget *parent) : QWidget(parent),
                                            boardSize(10),
                                            numMines(10),
                                            buttonHeight(30),
                                            buttonWidth(30) {
    // Set default board size and number of mines
    boardSize = 10;
    numMines = 10;
    debugMode = false;

    // Initialize grid layout
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);

    // Setup the game board
    setupBoard();

    // Load game state from .ini file
    loadGame();
}

Minesweeper::~Minesweeper() {
    // Cleanup resources
}

void Minesweeper::setupBoard() {
    gridLayout = new QGridLayout(this);

    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            buttons[i][j] = new QPushButton(this);
            buttons[i][j]->setFixedSize(buttonWidth, buttonHeight);
            connect(buttons[i][j], &QPushButton::clicked, this, &Minesweeper::cellClicked);
            gridLayout->addWidget(buttons[i][j], i, j);
        }
    }

    setLayout(gridLayout);
}


void Minesweeper::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // Left click: Reveal the cell
        // Calculate which cell was clicked
        int row = event->y() / buttonHeight; // Calculate row based on y coordinate
        int col = event->x() / buttonWidth; // Calculate column based on x coordinate

        if (row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
            // Ensure the click is within the grid bounds
            if (mines[row][col] == 1) {
                // Game over: Hit a mine
                gameOver(false); // Just end the game on loss
            } else {
                revealCell(row, col);
                checkWin();
            }
        }
    } else if (event->button() == Qt::RightButton) {
        // Right click: Flag/unflag the cell
        // Implement flagging logic as needed
    }
}

void Minesweeper::newGame() {
    // Start a new game with custom settings
    // ...
}

void Minesweeper::cellClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (!clickedButton) return;

    int row = -1, col = -1;
    // Find the clicked button's position in the grid
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (buttons[i][j] == clickedButton) {
                row = i;
                col = j;
                break;
            }
        }
        if (row != -1) break;// Break if the button is found
    }

    if (row == -1 || col == -1) return;// Button not found

    if (revealed[row][col]) return;// Cell already revealed

    if (QApplication::mouseButtons() == Qt::RightButton) {
        // Right click: Flag/unflag the cell
        flagged[row][col] = !flagged[row][col];
        if (flagged[row][col]) {
            buttons[row][col]->setText("F");
        } else {
            buttons[row][col]->setText("");
        }
    } else if (QApplication::mouseButtons() == Qt::LeftButton) {
        // Left click: Reveal the cell
        if (mines[row][col] == 1) {
            // Game over: Hit a mine
            gameOver(false);
        } else {
            revealCell(row, col);
            checkWin();
        }
    }
}

void Minesweeper::placeMines(int rowClicked, int colClicked) {
    // Place mines on the board after the first click
    int minesPlaced = 0;
    while (minesPlaced < numMines) {
        int row = QRandomGenerator::global()->bounded(0, boardSize);
        int col = QRandomGenerator::global()->bounded(0, boardSize);
        if (row != rowClicked && col != colClicked && mines[row][col] != 1) {
            // Ensure the clicked cell and its neighbors are safe
            mines[row][col] = 1;
            ++minesPlaced;
        }
    }
}

void Minesweeper::revealCell(int row, int col) {
    // Reveal cell and its neighbors
    revealed[row][col] = true;
    buttons[row][col]->setEnabled(false);

    if (mines[row][col] == 0) {
        // Empty cell: recursively reveal neighbors
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nr = row + dr;
                int nc = col + dc;
                if (nr >= 0 && nr < boardSize && nc >= 0 && nc < boardSize && !revealed[nr][nc]) {
                    revealCell(nr, nc);
                }
            }
        }
    } else {
        // Non-empty cell: reveal its value
        buttons[row][col]->setText(QString::number(mines[row][col]));
    }
}

void Minesweeper::checkWin() {
    // Check if the player has won the game
    bool win = true;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (mines[i][j] != 1 && !revealed[i][j]) {
                win = false;
                break;
            }
        }
        if (!win) break;
    }
    if (win) gameOver(true);
}

void Minesweeper::gameOver(bool win) {
    // Handle game over (win or lose)
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            revealed[i][j] = true;
            buttons[i][j]->setEnabled(false);
            if (mines[i][j] == 1) {
                buttons[i][j]->setText("X");
                if (!win && i == row && j == col) {
                    // Highlight the last clicked mine
                    buttons[i][j]->setStyleSheet("background-color: red; border: 1px solid black;");
                }
            } else {
                buttons[i][j]->setText(QString::number(mines[i][j]));
            }
        }
    }

    QString message = win ? "Congratulations! You won the game!" : "Game over! You hit a mine!";
    QMessageBox::information(this, "Game Over", message);

    // Save current game state to .ini file
    saveGame();
}

void Minesweeper::saveGame() {
    // Save current game state to .ini file
    QString filePath = QDir::currentPath() + "/minesweeper.ini";
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "BoardSize=" << boardSize << "\n";
        out << "NumMines=" << numMines << "\n";
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                out << revealed[i][j] << " ";
                out << flagged[i][j] << " ";
            }
            out << "\n";
        }
        file.close();
    }
}

void Minesweeper::loadGame() {
    // Load game state from .ini file
    QString filePath = QDir::currentPath() + "/minesweeper.ini";
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();
        QStringList parts = line.split("=");
        if (parts.size() == 2) {
            boardSize = parts[1].toInt();
        }
        line = in.readLine();
        parts = line.split("=");
        if (parts.size() == 2) {
            numMines = parts[1].toInt();
        }
        for (int i = 0; i < boardSize; ++i) {
            line = in.readLine();
            parts = line.split(" ", QString::SkipEmptyParts);
            for (int j = 0; j < boardSize; ++j) {
                if (parts.size() >= 2) {
                    revealed[i][j] = parts[0].toInt();
                    flagged[i][j] = parts[1].toInt();
                }
            }
        }
        file.close();
    }
}
