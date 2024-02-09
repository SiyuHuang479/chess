#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include "levelone.h"
using namespace std;

const int boardDim = 8;

LevelOne::LevelOne(string name, PieceColour colour, Board* board): 
    Computer{name, colour, board, Level::Level1} {}

bool LevelOne::makeMove() {
    // if curr side stalemate, cannot move
    if (board->isStalemate(colour)) return false;

    // record positions of own side chess
    vector<vector<int>> myChessList;
    // record coordinates of empty / opponent occupied grid
    vector<vector<int>> availableGrid;
    for (int i = 0; i < boardDim; i++) {
        for (int j = 0; j < boardDim; j++) {
            if (!board->returnChess(i, j)->isEmpty() &&
                board->returnChess(i, j)->returnColour() == colour) {
                myChessList.emplace_back(vector<int>{i,j});
            }
            else availableGrid.emplace_back(vector<int>{i,j});
        }
    }

    // reference: https://cplusplus.com/reference/algorithm/shuffle/
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // get a random list of own side chess
    shuffle(myChessList.begin(), myChessList.end(), default_random_engine(seed));
    int curChess = 0;
    // get the first chess coords
    int x1 = myChessList[curChess][0];
    int y1 = myChessList[curChess][1];
    //cout << "get random chess (" << x1 << ", " << y1 << ")" << endl;

    // list to store all the legal moves of current chess
    vector<vector<int>> legalMoves;
    for (size_t i = 0; i < availableGrid.size(); i++) {
        int x2 = availableGrid[i][0];
        int y2 = availableGrid[i][1];
        if (board->canMove(x1, y1, x2, y2)) {
            legalMoves.emplace_back(vector<int>{x2, y2}); 
        }
    }

    // if there are no legal moves for curr chess, change one and check again
    while (legalMoves.empty()) {
        // choose the next chess
        curChess++;
        x1 = myChessList[curChess][0];
        y1 = myChessList[curChess][1];

        for (size_t i = 0; i < availableGrid.size(); i++) {
            int x2 = availableGrid[i][0];
            int y2 = availableGrid[i][1];
            if (board->canMove(x1, y1, x2, y2)) {
               legalMoves.emplace_back(vector<int>{x2, y2}); 
            }
        }
    }

    // reference: https://cplusplus.com/reference/cstdlib/rand/
    // get a random legal move
    int moveNum = rand() % (legalMoves.size());
    int x2 = legalMoves[moveNum][0];
    int y2 = legalMoves[moveNum][1];
    //cout << "get random move (" << x2 << ", " << y2 << ")" << endl;
    
    // perform pawn promotion if valid
    if (isPromotion(x1, y1, x2, y2)) {
        int pieceNum = rand() % (4);
        if (colour == PieceColour::White) {
            vector<string> replacePiece {"R", "N", "B", "Q"};
            board->placeChess(replacePiece[pieceNum], x2, y2);
        } else {
            vector<string> replacePiece {"r", "n", "b", "q"};
            board->placeChess(replacePiece[pieceNum], x2, y2);
        }
        board->removeChess(x1, y1);
        return true;
    }
    board->move(x1, y1, x2, y2);
    return true;
}
