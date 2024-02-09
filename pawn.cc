#include <string>
#include <iostream>
#include "pawn.h"
#include "board.h"

Pawn::Pawn(Board* board, PieceColour colour) : Chess{board, colour, false} {
      setDescription("pawn");
}

Pawn::~Pawn() { }

bool Pawn::isEmpty() const { return empty; }

bool Pawn::isBlocked(int x1, int y1, int x2, int y2) const {
    // block only happens if the pawn moves forward (not diagonally, because they take the piece)
    if (isCapture(x1, y1, x2, y2)) return false;
    if (colour == PieceColour::Black) { // for black pawns, x increments
        do {
            x1 += 1;
            if (x1 > 7 || x1 < 0) return false; // out of bounds
            if (!board->isEmpty(x1, y1)) return true; //  to figure out in board   // is this return correct?
        } while (x2 != x1);
    } else { // for white pawns, x decrements
        do {
            x1 -= 1;
            if (x1 > 7 || x1 < 0) return false; // out of bounds
            if (!board->isEmpty(x1, y1)) return true; //  to figure out in board   // is this return correct?
        } while (x2 != x1);
    }
    return false;
}

bool Pawn::isCapture(int x1, int y1, int x2, int y2) const {
    if (colour == PieceColour::Black) {
        if (!board->isEmpty(x2, y2) && !(board->returnChess(x2, y2)->returnColour() == colour) && abs(y2 - y1) == 1 && x2 - x1 == 1) {
            return true;
        } else if (isEnPassant(x1, y1, x2, y2)) {
            return true;
        }
    }
    if (colour == PieceColour::White) {
        if (abs(y2 - y1) == 1 && x2 - x1 == -1 && !board->isEmpty(x2, y2) && !(board->returnChess(x2, y2)->returnColour() == colour) && abs(y2 - y1) == 1 && x2 - x1 == -1) {
                return true;
            } else if (isEnPassant(x1, y1, x2, y2)) {
                return true;
        }
    }
    return false;
}

// bool Pawn::isFirstMove() const { return firstMove; }

// bool Pawn::isEndOfBoard() const {
//     return (getY() == 0 || getY() == 7);
// }

bool Pawn::isLegalMove(int x1, int y1, int x2, int y2) const {
    if (x2 < 0 || x2 > 7 || y1 < 0 || y1 > 7) return false; // when coords are out of bounds

    if (abs(x2 - x1) > 1 && getMoveNum() != 0) { return false; } // more than one vertical square and not the first move

    if (abs(y1 - y2) > 1) return false; // more than one horizontal square

    if (abs(x2 - x1) == 2 && abs(y1 - y2) == 1) { return false; } // two vertical square and one horizontal square not allowed at all times

    // if (abs(x2 - x1) == 2 && !isCapture(x1, y1, x2, y2)) return false; // if moving vertically twice when not capturing

    if (y1 != y2 && !isCapture(x1, y1, x2, y2)) return false; // if moving horizontally when not capturing

    if (colour == PieceColour::Black && x2 - x1 < 0) { // x-coord needs to increment for black pieces
        return false; 
    } else if (colour == PieceColour::Black && x2 - x1 > 2) {
        return false; // move more than two vertical square regardless of move num
    } else if (colour == PieceColour::Black && x2 - x1 > 1 && getMoveNum() != 0) { 
        return false; 
    } // more than one vertical square and not the first move)

    if (colour == PieceColour::White && x2 - x1 > 0) { // x-coord needs to decrement for white pieces
        return false; 
    } else if (colour == PieceColour::White && x2 - x1 < -2) {
        return false; // move more than two vertical square regardless of move num
    } else if (colour == PieceColour::White &&  x2 - x1 < -1 && getMoveNum() != 0) { 
        return false;
    } // more than one vertical square and not the first move)
    
    if (x2 - x1 == 0) return false; // piece is not moving vertically at all
    return true;
}

bool Pawn::isEndOfBoard() const { 
    if (colour == PieceColour::Black && getX() == 0) { // for black piece
        return true; 
    }

    if (colour == PieceColour::White && getX() == 7) { // for white piece
        return true; 
    } 

    return false;
}


bool Pawn::isEnPassant(int x1, int y1, int x2, int y2) const {
    if (colour == PieceColour::Black) {
        if (abs(y2 - y1) == 1 && x2 - x1 == 1 && !board->returnChess(x2 - 1, y2)->isEmpty() && board->returnChess(x2 - 1, y2)->returnDescription() == "pawn" && board->returnChess(x2 - 1, y2)->returnColour() != colour && board->returnChess(x2 - 1, y2)->getMoveNum() == 1) {
            return true;
        }
    }
    if (colour == PieceColour::White) {
        if (abs(y2 - y1) == 1 && x2 - x1 == -1 && !board->returnChess(x2 + 1, y2)->isEmpty() && board->returnChess(x2 + 1, y2)->returnDescription() == "pawn" && board->returnChess(x2 + 1, y2)->returnColour() != colour && board->returnChess(x2 + 1, y2)->getMoveNum() == 1) {
                return true;
        }
    }
    return false;
}
