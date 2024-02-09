#include "board.h"
#include "chess.h"
using namespace std;

Board::Board() : gameState{GameState::Ongoing} {
    for (int i = 0; i < 8; ++i) {
       vector<unique_ptr<Chess>> row;
        for (int j = 0; j < 8; ++j) {
            // initializes all squares of the board with empty squares,
            // wait until setup mode is successful to populate board
            // row.emplace_back(new Empty(this, PieceColour::Empty));
            unique_ptr<Chess> item = make_unique<Empty>(this, PieceColour::Empty);
            row.emplace_back(std::move(item));
        }
        board.emplace_back(std::move(row));
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j]->setCoords(i, j);
            board[i][j]->attach(td);
            board[i][j]->attach(gd);
        }
    }
}

GameState Board::returnGamestate() const {
    return gameState;
}

void Board::setLastMove(vector<vector<int>> move) {
    lastMove = move;
}

const vector<vector<int>> Board::getLastMove() const {
    return lastMove;
}

bool Board::isBlocked(int x1, int y1, int x2, int y2) const {
    return board[x1][y1]->isBlocked(x1, y1, x2, y2);
}

bool Board::canMove(int x1, int y1, int x2, int y2) const {

    PieceColour curColour = board[x1][y1]->returnColour();
    // check whether current player move the correct colour

    bool legal = board[x1][y1]->isLegalMove(x1, y1, x2, y2);
    if (!legal) return false;

    bool blocked = board[x1][y1]->isBlocked(x1, y1, x2, y2);
    if (blocked) return false;

    if (board[x2][y2]->returnColour() == board[x1][y1]->returnColour()) return false;
    
    PieceColour isCheckedColour = isCheck(curColour);
    
    // locate curColour's king location
    int KingX, KingY;
    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            if (!board[i][k]->isEmpty() && board[i][k]->returnDescription() == "king" && board[i][k]->returnColour() == curColour) {
                KingX = i;
                KingY = k;
                break;
            }
        }
    }

    // if the chess to be moved is king
    if (board[x1][y1]->returnDescription() == "king") {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // if there is at least one opponent's chess can move to the new location of our king
                if (!board[i][j]->isEmpty() && board[i][j]->returnColour() != curColour) {
                    if (board[i][j]->isLegalMove(i, j, x2, y2) && !board[i][j]->isBlocked(i, j, x2, y2)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    // temporarily model after move, check if this move cause my king to be in Check
    bool beforeEmpty1 = board[x1][y1]->isEmpty();
    bool beforeEmpty2 = board[x2][y2]->isEmpty();
    PieceColour beforeEmptyC1 = board[x1][y1]->returnColour();
    PieceColour beforeEmptyC2 = board[x2][y2]->returnColour();
    board[x1][y1]->setColor(PieceColour::Empty);
    board[x2][y2]->setColor(beforeEmptyC1);
    board[x1][y1]->setEmpty(true);
    board[x2][y2]->setEmpty(false);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!(board[i][j]->isEmpty()) && board[i][j]->returnColour() != curColour && board[i][j]->returnColour() != PieceColour::Empty) {
                // after move, some opponent's chess can move to curColour's King location
                if (board[i][j]->isLegalMove(i, j, KingX, KingY) && !board[i][j]->isBlocked(i, j, KingX, KingY)) {
                    board[x1][y1]->setEmpty(beforeEmpty1);
                    board[x2][y2]->setEmpty(beforeEmpty2);
                    board[x1][y1]->setColor(beforeEmptyC1);
                    board[x2][y2]->setColor(beforeEmptyC2);
                    return false;
                }
            }
        }
    }

    // restore the initial status
    board[x1][y1]->setEmpty(beforeEmpty1);
    board[x2][y2]->setEmpty(beforeEmpty2);
    board[x1][y1]->setColor(beforeEmptyC1);
    board[x2][y2]->setColor(beforeEmptyC2);

    /*
    // if curColour is being checked
    if (isCheckedColour == curColour) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // if there is at least one opponent's chess can move to the new location of our king
                if (!board[i][j]->isEmpty() && board[i][j]->returnColour() != curColour) {
                    if (board[i][j]->isLegalMove(i, j, x2, y2) && !board[i][j]->isBlocked(i, j, x2, y2)) {
                        return false;
                    }
                }
            }
        }
    }
    */

    return true;
 }

void Board::move (int x1, int y1, int x2, int y2) {
    if (!board[x2][y2]->isEmpty()) {
        
        board[x1][y1]->addMoveNum();
        // if there is a piece at destination square, capture
        swap(board[x1][y1], board[x2][y2]);
        removeChess(x1, y1);

    } else if (board[x2][y2]->isEmpty()) {
        if (board[x1][y1]->returnDescription() == "pawn" && dynamic_cast<Pawn*>(board[x1][y1].get())->isEnPassant(x1, y1, x2, y2)) {
            // delete board[x1][y1];
            if (board[x1][y1]->returnColour() == PieceColour::White && x2 == lastMove[1][0] - 1 && y2 == lastMove[1][1]) {
                removeChess(x2 + 1, y2);
            } else if (board[x1][y1]->returnColour() == PieceColour::Black && x2 == lastMove[1][0] + 1 && y2 == lastMove[1][1]) {
                removeChess(x2 - 1, y2);
            }
            swap(board[x1][y1], board[x2][y2]);
            // board[x2][y2] = board[x1][y1];
        }
        else if (board[x1][y1]->returnDescription() == "king" && dynamic_cast<King*>(board[x1][y1].get())->checkCastling(x1, y1, x2, y2)) {
            // castling moves
            int rookX1 = x1;
            int rookY1;
            int rookX2 = x2;
            int rookY2;
            if (y2 == 6) {
                rookY1 = y1 + 3;
                rookY2 = y2 - 1;
                swap(board[x1][y1], board[x2][y2]);
                swap(board[rookX1][rookY1], board[rookX2][rookY2]);
            } else {
                y2 = 2;
                rookY1 = y1 - 4;
                rookY2 = y2 + 1;
                swap(board[x1][y1], board[x2][y2]);
                swap(board[rookX1][rookY1], board[rookX2][rookY2]);
            }
            removeChess(x1, y1);
            removeChess(rookX1, rookY1);
            board[x1][y1]->setCoords(x1, y1);
            board[x2][y2]->setCoords(x2, y2);
            board[rookX1][rookY1]->setCoords(rookX1, rookY1);
            board[rookX2][rookY2]->setCoords(rookX2, rookY2);
            // returnChess(x1, y1)->notifyObservers(x1, y1, x1, y1);
            returnChess(x2, y2)->notifyObservers(x2, y2, x2, y2);
            // returnChess(rookX1, rookY1)->notifyObservers(rookX1, rookY1, rookX1, rookY1);
            returnChess(rookX2, rookY2)->notifyObservers(rookX2, rookY2, rookX2, rookY2);
            board[rookX2][rookY2]->addMoveNum();
            board[x2][y2]->addMoveNum();
            return;
        } else {
            // the square is empty, swap
            board[x1][y1]->addMoveNum();
            swap(board[x1][y1], board[x2][y2]);
        }
        // returnChess(x2, y2)->notifyObservers(x1, y1, x2, y2);
        
    }
    board[x1][y1]->setCoords(x1, y1);
    board[x2][y2]->setCoords(x2, y2);
    returnChess(x2, y2)->notifyObservers(x1, y1, x2, y2);

    setLastMove({{x1, y1}, {x2, y2}});
}

void Board::notify(Chess &piece, int x1, int y1, int x2, int y2) { 
    // int x = piece.getX();
    // int y = piece.getY();
    // delete board[x][y];
    // board[x][y] = piece; // update board at square (x,y)
}

bool Board::isEmpty(int x, int y) const { return board[x][y]->isEmpty(); }

void Board::clearBoard() { 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // delete board[i][j];
            board[i][j] = nullptr;
        }
    }
    // delete td;
 }

PieceColour Board::isCheck(PieceColour curColour) const { 
    int opKingX, opKingY;
    int curKingX, curKingY;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // locate opponent king's position
            if (board[i][j]->returnDescription() == "king" && board[i][j]->returnColour() != curColour) {
                opKingX = i;
                opKingY = j;
                if (curColour == PieceColour::Black) {
                    // std::cout << "white king is at " << i << ", " << j << endl;
                } else {
                    // std::cout << "black king is at " << i << ", " << j << endl;
                }
            }

            // locate current colour king's position
            if (board[i][j]->returnDescription() == "king" && board[i][j]->returnColour() == curColour) {
                curKingX = i;
                curKingY = j;
                if (curColour == PieceColour::Black) {
                    // std::cout << "black king is at " << i << ", " << j << endl;
                } else {
                    // std::cout << "white king is at " << i << ", " << j << endl;
                }
            }
        }
    } 

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // check if move will cause opponent to check me
            // I think opponent's king can also check my king?
            // if (!board[i][j]->isEmpty() && board[i][j]->returnDescription() != "king" && board[i][j]->returnColour() != curColour) {
            if (!board[i][j]->isEmpty() && board[i][j]->returnColour() != curColour) {
                if (board[i][j]->isLegalMove(i,j,curKingX, curKingY) && !board[i][j]->isBlocked(i,j,curKingX, curKingY)) {
                    if (curColour == PieceColour::Black) {
                        // std::cout << "checking black king at " << i << ", " << j << endl;
                    } else {
                        // std::cout << "checking white king at " << i << ", " << j << endl;
                    }
                    return curColour;
                }
            }
            
            // check if move will cause me to check opponent
            if (!board[i][j]->isEmpty() && board[i][j]->returnDescription() != "king" && board[i][j]->returnColour() == curColour) {
                 if (board[i][j]->isLegalMove(i,j,opKingX, opKingY) && !board[i][j]->isBlocked(i,j,opKingX, opKingY)) {
                    if (curColour == PieceColour::Black) {
                        // std::cout << "checking white king at " << i << ", " << j << endl;
                    } else {
                        // std::cout << "checking black king at " << i << ", " << j << endl;
                    }
                    return PieceColour::Black == curColour ? 
                                                 PieceColour::White : PieceColour::Black;
                }
            }
            
        }
    }
    return PieceColour::Empty;
    
 }

// check if for all canmoves, if there is no more that will return ischeck false
bool Board::isStalemate(PieceColour curColour) {
    for (int curX = 0; curX < 8; ++curX) {
        for (int curY = 0; curY < 8; ++curY) {
            if (board[curX][curY]->returnColour() == curColour) { 
                // check only current turn player's pieces
                for (int destX = 0; destX < 8; ++destX) {
                    for (int destY = 0; destY < 8; ++destY) {
                        // if at least one legal move that doesn't result in check, not stalemate
                        if (canMove(curX, curY, destX, destY)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    setGameState(GameState::Stalemate);
    return true; // true if no legal moves found for any piece of the current turn
}
bool Board::isCheckmate(PieceColour curColour) {
    if (isCheck(curColour) == PieceColour::Empty) return false;
    if (isCheck(curColour) == curColour) {
        for (int curX = 0; curX < 8; ++curX) {
            for (int curY = 0; curY < 8; ++curY) {
                if (board[curX][curY]->returnColour() == curColour) { 
                    // check current turn player's pieces
                    for (int destX = 0; destX < 8; ++destX) {
                        for (int destY = 0; destY < 8; ++destY) {
                            // if at least one legal move that doesn't result in check, not checkmate
                            if (canMove(curX, curY, destX, destY)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    if (isCheck(curColour) != curColour) {
        for (int curX = 0; curX < 8; ++curX) {
            for (int curY = 0; curY < 8; ++curY) {
                if (board[curX][curY]->returnColour() != curColour) { 
                    // check opponent turn player's pieces
                    for (int destX = 0; destX < 8; ++destX) {
                        for (int destY = 0; destY < 8; ++destY) {
                            // if at least one legal move that doesn't result in check, not checkmate
                            if (canMove(curX, curY, destX, destY)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    if (isCheck(curColour) == curColour && curColour == PieceColour::White) {
        setGameState(GameState::BlackWins);
    } else if (isCheck(curColour) == curColour && curColour == PieceColour::Black) {
        setGameState(GameState::WhiteWins);
    } else if (isCheck(curColour) != curColour && curColour == PieceColour::White) {
        setGameState(GameState::WhiteWins);
    } else if (isCheck(curColour) != curColour && curColour == PieceColour::Black) {
        setGameState(GameState::BlackWins);
    }
    return true; // same as isStalemate, but runs only when isCheck is not Empty
}

Chess* Board::returnChess(int x, int y) {
    return board[x][y].get();
}

void Board::setGameState(GameState state) {
    this->gameState = state;
}

void Board::placeChess(string piece, int x, int y) {

    // delete board[x][y];

    if (piece == "K") {
        // board[x][y] = new King(this, PieceColour::White);
        board[x][y] = make_unique<King>(this, PieceColour::White);
        board[x][y]->setDescription("king");
    }
    else if (piece == "k") {
        // board[x][y] = new King(this, PieceColour::Black);
        board[x][y] = make_unique<King>(this, PieceColour::Black);
        board[x][y]->setDescription("king");
    }
    else if (piece == "Q") {
        // board[x][y] = new Queen(this, PieceColour::White);
        board[x][y] = make_unique<Queen>(this, PieceColour::White);
        board[x][y]->setDescription("queen");
    }
    else if (piece == "q") {
        // board[x][y] = new Queen(this, PieceColour::Black);
        board[x][y] = make_unique<Queen>(this, PieceColour::Black);
        board[x][y]->setDescription("queen");
    }
    else if (piece == "B") {
        // board[x][y] = new Bishop(this, PieceColour::White);
        board[x][y] = make_unique<Bishop>(this, PieceColour::White);
        board[x][y]->setDescription("bishop");
    }
    else if (piece == "b") {
        // board[x][y] = new Bishop(this, PieceColour::Black);
        board[x][y] = make_unique<Bishop>(this, PieceColour::Black);
        board[x][y]->setDescription("bishop");
    }
    else if (piece == "P") {
        // board[x][y] = new Pawn(this, PieceColour::White);
        board[x][y] = make_unique<Pawn>(this, PieceColour::White);
        board[x][y]->setDescription("pawn");
    }
    else if (piece == "p") {
        // board[x][y] = new Pawn(this, PieceColour::Black);
        board[x][y] = make_unique<Pawn>(this, PieceColour::Black);
        board[x][y]->setDescription("pawn");
    }
    else if (piece == "R") {
        // board[x][y] = new Rook(this, PieceColour::White);
        board[x][y] = make_unique<Rook>(this, PieceColour::White);
        board[x][y]->setDescription("rook");
    }
    else if (piece == "r") {
        // board[x][y] = new Rook(this, PieceColour::Black);
        board[x][y] = make_unique<Rook>(this, PieceColour::Black);
        board[x][y]->setDescription("rook");
    }
    else if (piece == "N") {
        // board[x][y] = new Knight(this, PieceColour::White);
        board[x][y] = make_unique<Knight>(this, PieceColour::White);
        board[x][y]->setDescription("knight");
    }
    else if (piece == "n") {
        // board[x][y] = new Knight(this, PieceColour::Black);
        board[x][y] = make_unique<Knight>(this, PieceColour::Black);
        board[x][y]->setDescription("knight");
    }
    else {
        board[x][y] = make_unique<Empty>(this, PieceColour::Empty);
        board[x][y]->setDescription("empty");
    }
    board[x][y]->setCoords(x, y);
    board[x][y]->attach(td);
    board[x][y]->attach(gd);
    board[x][y]->notifyObservers(x, y, x, y);

}

void Board::removeChess(int x, int y) {
    // if (board[x][y]->isEmpty()) return;
    // delete board[x][y];
    // board[x][y] = new Empty(this, PieceColour::Empty);
    board[x][y] = make_unique<Empty>(this, PieceColour::Empty);
    board[x][y]->setDescription("empty");
    board[x][y]->setCoords(x, y);
    board[x][y]->attach(td);
    board[x][y]->attach(gd);
    board[x][y]->notifyObservers(x, y, x, y);
}


bool Board::isLegalSetup() const {
    bool whiteKing = false;
    bool blackKing = false;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j]->returnDescription() == "king") {
                if (board[i][j]->returnColour() == PieceColour::White) {
                    if (whiteKing) return false;
                    whiteKing = true;
                }
                else {
                    if (blackKing) return false;
                    blackKing = true;
                }
            }
            if (i == 0 || i == 7) {
                if (board[i][j]->returnDescription() == "pawn") return false;
            }
        }
    }
    return whiteKing && blackKing && (isCheck(PieceColour::Black) == PieceColour:: Empty) && (isCheck(PieceColour::White) == PieceColour:: Empty);
}

void Board::attachTD(TextDisplay* td) {
    this->td = td;
}

void Board::attachGD(Xwindow* gd) {
    this->gd = gd;
}
