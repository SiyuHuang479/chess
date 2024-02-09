#include <string>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <algorithm>
#include "levelfour.h"

using namespace std;

const int boardDim = 8;

LevelFour::LevelFour(string name, PieceColour colour, Board* board): 
    Computer{name, colour, board, Level::Level4} {
    weightedMove["capPawn"] = 1;
    weightedMove["capKnight"] = 3;
    weightedMove["capBishop"] = 3;
    weightedMove["capRook"] = 5;
    weightedMove["capQueen"] = 9;

    weightedMove["-Pawn"] = -1;
    weightedMove["-Knight"] = -3;
    weightedMove["-Bishop"] = -3;
    weightedMove["-Rook"] = -5;
    weightedMove["-Queen"] = -9;
}

bool LevelFour::inCapture(int x, int y, const vector<vector<int>> opChess) { 
    for (size_t i = 0; i < opChess.size(); i++) {
        int opX = opChess[i][0];
        int opY = opChess[i][1];
        // if curr opchess being captured
        if (opX == x && opY == y) {
            continue;
        }
        // check pawn capture
        if (board->returnChess(opX, opY)->returnDescription() == "pawn" &&
            isPawnCapture(opX, opY, x, y)) {
            return true;
        } 
        // check normal capture
        if (board->canMove(opX, opY, x, y)) {
            return true;
        } else continue;
    }
    // not in capture, safe position
    return false;
}

bool LevelFour::checkEscape(int kingX, int kingY, const vector<vector<int>> opChess, const vector<vector<int>> grid) {
    for (size_t i = 0; i < grid.size(); i++) {
        int x2 = grid[i][0];
        int y2 = grid[i][1];
        if (board->canMove(kingX, kingY, x2, y2)) {
            // check potential capture at (x2, y2)
            PieceColour prevDestCor = board->returnChess(x2, y2)->returnColour();
            bool prevDestEptState = board->returnChess(x2, y2)->isEmpty();
            board->returnChess(x2, y2)->setColor(board->returnChess(kingX, kingY)->returnColour());
            board->returnChess(x2, y2)->setEmpty(false);

            PieceColour prevStartCor = board->returnChess(kingX, kingY)->returnColour();
            bool prevStartEptState = board->returnChess(kingX, kingY)->isEmpty();
            board->returnChess(kingX, kingY)->setColor(PieceColour::Empty);
            board->returnChess(kingX, kingY)->setEmpty(true);

            if (!inCapture(x2, y2, opChess)) {
                board->returnChess(x2, y2)->setColor(prevDestCor);
                board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                board->returnChess(kingX, kingY)->setColor(prevStartCor);
                board->returnChess(kingX, kingY)->setEmpty(prevStartEptState);
                board->move(kingX, kingY, x2, y2);
                return true;
            } else {
                // restore and continue
                board->returnChess(x2, y2)->setColor(prevDestCor);
                board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                board->returnChess(kingX, kingY)->setColor(prevStartCor);
                board->returnChess(kingX, kingY)->setEmpty(prevStartEptState);
                continue;
            }
        }
    }
    // check king's legal move
    for (size_t i = 0; i < grid.size(); i++) {
        int x2 = grid[i][0];
        int y2 = grid[i][1];
        if (board->canMove(kingX, kingY, x2, y2)) {
            board->move(kingX, kingY, x2, y2);
            return true;
        }
    }
    return false;
}

void LevelFour::addScore(int x1, int y1, int x2, int y2, map<vector<int>, int>& moveScore, bool byStartPos) {
    if (byStartPos) {
        if (board->returnChess(x1, y1)->returnDescription() == "queen") {
            moveScore[{x1, y1, x2, y2}] += weightedMove["capQueen"];
        } 
        if (board->returnChess(x1, y1)->returnDescription() == "rook") {
            moveScore[{x1, y1, x2, y2}] += weightedMove["capRook"];
        }
        if (board->returnChess(x1, y1)->returnDescription() == "bishop") {
            moveScore[{x1, y1, x2, y2}] += weightedMove["capBishop"];
        }
        if (board->returnChess(x1, y1)->returnDescription() == "knight") {
            moveScore[{x1, y1, x2, y2}] += weightedMove["capKnight"];
        }
        if (board->returnChess(x1, y1)->returnDescription() == "pawn") {
            moveScore[{x1, y1, x2, y2}] += weightedMove["capPawn"];
        }
    } else {
        if (board->returnChess(x2, y2)->returnDescription() == "queen") {
            moveScore[{x1, y1, x2, y2}] += weightedMove["capQueen"];
        } 
        if (board->returnChess(x2, y2)->returnDescription() == "rook") {
            moveScore[{x1, y1, x2, y2}] += weightedMove["capRook"];
        }
        if (board->returnChess(x2, y2)->returnDescription() == "bishop") {
            moveScore[{x1, y1, x2, y2}] += weightedMove["capBishop"];
        }
        if (board->returnChess(x2, y2)->returnDescription() == "knight") {
            moveScore[{x1, y1, x2, y2}] += weightedMove["capKnight"];
        }
        if (board->returnChess(x2, y2)->returnDescription() == "pawn") {
            moveScore[{x1, y1, x2, y2}] += weightedMove["capPawn"];
        }
    }
}

void LevelFour::deductScore(int x1, int y1, int x2, int y2, map<vector<int>, int>& moveScore) {
    if (board->returnChess(x1, y1)->returnDescription() == "queen") {
        moveScore[{x1, y1, x2, y2}] += weightedMove["-Queen"];
    } 
    if (board->returnChess(x1, y1)->returnDescription() == "rook") {
        moveScore[{x1, y1, x2, y2}] += weightedMove["-Rook"];
    }
    if (board->returnChess(x1, y1)->returnDescription() == "bishop") {
        moveScore[{x1, y1, x2, y2}] += weightedMove["-Bishop"];
    }
    if (board->returnChess(x1, y1)->returnDescription() == "knight") {
        moveScore[{x1, y1, x2, y2}] += weightedMove["-Knight"];
    }
    if (board->returnChess(x1, y1)->returnDescription() == "pawn") {
        moveScore[{x1, y1, x2, y2}] += weightedMove["-Pawn"];
    }
}

bool LevelFour::makeMove() {
    // record positions of own side chess
    vector<vector<int>> myChessList;
    // record positions of opponent's chess
    vector<vector<int>> opChessList;
    // record coordinates of available grids (opponent occupied/empty)
    vector<vector<int>> availableGrid;
    // record coordinates of king
    int opKingX, opKingY;
    int myKingX, myKingY;

    map<vector<int>, int> moveScore;
    vector<vector<int>> key;

    for (int i = 0; i < boardDim; i++) {
        for (int j = 0; j < boardDim; j++) {
            if (!board->returnChess(i, j)->isEmpty() &&
                board->returnChess(i, j)->returnColour() == colour) {
                // if own side chess
                myChessList.emplace_back(vector<int>{i,j});
                // if my king
                if (board->returnChess(i, j)->returnDescription() == "king") {
                    myKingX = i;
                    myKingY = j;
                }
            } else if (!board->returnChess(i, j)->isEmpty() &&
                board->returnChess(i, j)->returnColour() != colour) {
                // if opponent's chess
                opChessList.emplace_back(vector<int> {i, j});
                if (board->returnChess(i, j)->returnDescription() == "king") {
                    opKingX = i;
                    opKingY = j;
                }
                // opponent occupied grid
                availableGrid.emplace_back(vector<int> {i, j});
            } else {
                // if empty grid
                availableGrid.emplace_back(vector<int> {i, j});
            }
        }
    }

    // check escape if currently in check
    if (inCapture(myKingX, myKingY, opChessList)) {
        if (checkEscape(myKingX, myKingY, opChessList, availableGrid)) {
            return true;
        } 
        // if king cannot escape, check whether other piece can block the check
        else {
            for (size_t i = 0; i < myChessList.size(); i++) {
                int x1, y1, x2, y2;
                x1 = myChessList[i][0];
                y1 = myChessList[i][1];
                for (size_t j = 0; j < availableGrid.size(); j++) {
                    x2 = availableGrid[j][0];
                    y2 = availableGrid[j][1];
                    if (board->canMove(x1, y1, x2, y2)) {
                        // restore and change color & empty state of dest chess to stimulate move
                        PieceColour prevDestCor = board->returnChess(x2, y2)->returnColour();
                        bool prevDestEptState = board->returnChess(x2, y2)->isEmpty();
                        board->returnChess(x2, y2)->setColor(board->returnChess(x1, y1)->returnColour());
                        board->returnChess(x2, y2)->setEmpty(false);

                        PieceColour prevStartCor = board->returnChess(x1, y1)->returnColour();
                        bool prevStartEptState = board->returnChess(x1, y1)->isEmpty();
                        board->returnChess(x1, y1)->setColor(PieceColour::Empty);
                        board->returnChess(x1, y1)->setEmpty(true);
                        // if king still in capture after chess move
                        if (inCapture(myKingX, myKingY, opChessList)) {
                            board->returnChess(x2, y2)->setColor(prevDestCor);
                            board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                            board->returnChess(x1, y1)->setColor(prevStartCor);
                            board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                            continue;
                        } else {
                            // restore the dest chess and move
                            board->returnChess(x2, y2)->setColor(prevDestCor);
                            board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                            board->returnChess(x1, y1)->setColor(prevStartCor);
                            board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                            board->move(x1, y1, x2, y2);
                        }
                    }
                }
            }
        }
    }

    // no legal move for king in check, find other legal moves

    // reference: https://cplusplus.com/reference/algorithm/shuffle/
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // get a random list of available move
    shuffle(availableGrid.begin(), availableGrid.end(), default_random_engine(seed));
    // get a random list of available move
    shuffle(availableGrid.begin(), availableGrid.end(), default_random_engine(seed));

    for (size_t i = 0; i < myChessList.size(); i++) {
        int x1, y1, x2, y2;
        x1 = myChessList[i][0];
        y1 = myChessList[i][1];
        for (size_t j = 0; j < availableGrid.size(); j++) {
            x2 = availableGrid[j][0];
            y2 = availableGrid[j][1];
            if (board->canMove(x1, y1, x2, y2)) {
                // check checkmate
                if (board->returnChess(x2, y2)->returnDescription() == "king") {
                    board->move(x1, y1, x2, y2);
                    return true;
                }

                // check check move
                // if can move from (x2, y2) to the opponent king's location
                else if (board->returnChess(x1, y1)->isLegalMove(x2, y2, opKingX, opKingY) &&
                    !board->returnChess(x1, y1)->isBlocked(x2, y2, opKingX, opKingY)) {
                    board->move(x1, y1, x2, y2);
                    return true;
                }

                // check capturing moves, give weighted score
                // check pawn capture
                else if (board->returnChess(x1, y1)->returnDescription() == "pawn" &&
                    isPawnCapture(x1, y1, x2, y2)) {
                    key.emplace_back(vector<int> {x1, y1, x2, y2});
                    // if capture (x2, y2) , determine by (x2, y2) weight
                    addScore(x1, y1, x2, y2, moveScore, false);
                    
                    // if currently in capture
                    if (inCapture(x1, y1, opChessList)) {
                        // check potential capture
                        PieceColour prevDestCor = board->returnChess(x2, y2)->returnColour();
                        bool prevDestEptState = board->returnChess(x2, y2)->isEmpty();
                        board->returnChess(x2, y2)->setColor(colour);
                        board->returnChess(x2, y2)->setEmpty(false);

                        PieceColour prevStartCor = board->returnChess(x1, y1)->returnColour();
                        bool prevStartEptState = board->returnChess(x1, y1)->isEmpty();
                        board->returnChess(x1, y1)->setColor(PieceColour::Empty);
                        board->returnChess(x1, y1)->setEmpty(true);
                        if (inCapture(x2, y2, opChessList)) {
                            // restore and deduct score
                            board->returnChess(x2, y2)->setColor(prevDestCor);
                            board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                            board->returnChess(x1, y1)->setColor(prevStartCor);
                            board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                            deductScore(x1, y1, x2, y2, moveScore);
                        } else {
                            board->returnChess(x2, y2)->setColor(prevDestCor);
                            board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                            board->returnChess(x1, y1)->setColor(prevStartCor);
                            board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                            // successfuly escape, plus (x1, y1) weight
                            addScore(x1, y1, x2, y2, moveScore, true);
                        }
                    }
                }

                // other chess capture
                else if (!board->returnChess(x2, y2)->isEmpty()) {
                    addScore(x1, y1, x2, y2, moveScore, false);
                    key.emplace_back(vector<int> {x1, y1, x2, y2});

                    // if currently in capture
                    if (inCapture(x1, y1, opChessList)) {
                        // check potential capture
                        PieceColour prevDestCor = board->returnChess(x2, y2)->returnColour();
                        bool prevDestEptState = board->returnChess(x2, y2)->isEmpty();
                        board->returnChess(x2, y2)->setColor(colour);
                        board->returnChess(x2, y2)->setEmpty(false);

                        PieceColour prevStartCor = board->returnChess(x1, y1)->returnColour();
                        bool prevStartEptState = board->returnChess(x1, y1)->isEmpty();
                        board->returnChess(x1, y1)->setColor(PieceColour::Empty);
                        board->returnChess(x1, y1)->setEmpty(true);
                        // will still be in capture
                        if (inCapture(x2, y2, opChessList)) {
                            // restore and deduct score
                            board->returnChess(x2, y2)->setColor(prevDestCor);
                            board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                            board->returnChess(x1, y1)->setColor(prevStartCor);
                            board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                            deductScore(x1, y1, x2, y2, moveScore);
                        } else {
                            board->returnChess(x2, y2)->setColor(prevDestCor);
                            board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                            board->returnChess(x1, y1)->setColor(prevStartCor);
                            board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                            // successfuly escape, plus (x1, y1) score
                            addScore(x1, y1, x2, y2, moveScore, true);
                        }
                    }
                }

                // legal move to empty grid
                else {
                    moveScore[{x1, y1, x2, y2}] = 0;
                    key.emplace_back(vector<int> {x1, y1, x2, y2});

                    // if currently in capture
                    if (inCapture(x1, y1, opChessList)) {
                        // check potential capture
                        PieceColour prevDestCor = board->returnChess(x2, y2)->returnColour();
                        bool prevDestEptState = board->returnChess(x2, y2)->isEmpty();
                        board->returnChess(x2, y2)->setColor(colour);
                        board->returnChess(x2, y2)->setEmpty(false);

                        PieceColour prevStartCor = board->returnChess(x1, y1)->returnColour();
                        bool prevStartEptState = board->returnChess(x1, y1)->isEmpty();
                        board->returnChess(x1, y1)->setColor(PieceColour::Empty);
                        board->returnChess(x1, y1)->setEmpty(true);
                        // will still be in capture
                        if (inCapture(x2, y2, opChessList)) {
                            // restore and deduct score
                            board->returnChess(x2, y2)->setColor(prevDestCor);
                            board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                            board->returnChess(x1, y1)->setColor(prevStartCor);
                            board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                            deductScore(x1, y1, x2, y2, moveScore);
                        } else {
                            board->returnChess(x2, y2)->setColor(prevDestCor);
                            board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                            board->returnChess(x1, y1)->setColor(prevStartCor);
                            board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                            // successfuly escape, plus (x1, y1) score
                            addScore(x1, y1, x2, y2, moveScore, true);
                        }
                    }
                }
            }
        }
    }

    if (moveScore.empty()) return false; // if no legal move
    else {
        shuffle(key.begin(), key.end(), default_random_engine(seed));
        // get the legal move with highest score
        int highestScoresofar = moveScore[key[0]];
        vector<int> highestKey = key[0];
        //if (key.empty()) cout << "empty key" << endl;
        for (size_t i = 1; i < key.size(); i++) {
            //if (key[i].empty()) cout << "empty key[" << i << "]" << endl;
            if (moveScore[key[i]] > highestScoresofar) {
                highestScoresofar = moveScore[key[i]];
                highestKey = key[i];
                //cout << key[i][0] << " " << key[i][1] << " " << key[i][2] << " " << key[i][3] << endl;
            }
        }
        int x1, y1, x2, y2;
        x1 = highestKey[0];
        y1 = highestKey[1];
        x2 = highestKey[2];
        y2 = highestKey[3];
        //cout << "move from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ")" << endl;

        // perform pawn promotion if valid (queen the best choice)
        if (isPromotion(x1, y1, x2, y2)) {
            if (colour == PieceColour::White) {
                board->placeChess("Q", x2, y2);
            } else {
                board->placeChess("q", x2, y2);
            }
            board->removeChess(x1, y1);
            return true;
        }
        board->move(x1, y1, x2, y2);
        return true;
    }
}
