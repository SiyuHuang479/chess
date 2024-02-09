#include "king.h"
#include "board.h"
using namespace std;

King::King(Board* board, PieceColour colour) : Chess{board, colour, false} {
    setDescription("king");
} 


bool King::isLegalMove(int x1, int y1, int x2, int y2) const {

    // if out of bound
    if (x2 < 0 || y2 < 0 || x2 > 7 || y2 > 7) return false;
    
    int Xdistance = x1 - x2;
    int Ydistance = y1 - y2;
    // normal move
    if ((Xdistance == 1 || Xdistance == 0 || Xdistance == -1) && (Ydistance == 1 || Ydistance == -1 || Ydistance == 0)) return true;
    // castling
    if ( (x2 == 7 || x2 == 0) && (Ydistance == 2 || Ydistance == -2) ) return checkCastling(x1, y1, x2, y2);
    return false;
}

bool King::isBlocked(int x1, int y1, int x2, int y2) const {
    if ( (x2 == 7 || x2 == 0) && (abs(y2 - y1) == 2)  && checkCastling(x1, y1, x2, y2)) return false;
    if (board->returnChess(x2, y2)->returnColour() == colour) return true;
    return false;
}

bool King::checkCastling(int x1, int y1, int x2, int y2) const {

    if (y2 == 6) y2 = 7;
    else y2 = 0;
    
    if (getMoveNum() != 0) return false;

    // if that is not rook
    if (!(board->returnChess(x2, y2)->returnDescription() == "rook")) return false;
    
    // check whether rook is firstMove
    if (board->returnChess(x2, y2)->getMoveNum() != 0) return false;

    // check whether it is blocked
    int eachY;
    if (y2 > y1) eachY = 1;
    else eachY = -1;
    y1 += eachY;
    while (y1 != y2) {
        if (!(board->isEmpty(x1, y1))) return false;
        y1 += eachY;
    }

    // check whether king is in check
    if (board->isCheck(colour) == colour) return false;
    
    return true;
}

bool King::isEmpty() const { return empty; }
