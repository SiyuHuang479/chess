#ifndef __KING_H__
#define __KING_H__
#include "chess.h"

class Board;

class King : public Chess {
  public:
    King(Board* board, PieceColour colour);
    bool isLegalMove(int x1, int y1, int x2, int y2) const override;
    bool isBlocked(int x1, int y1, int x2, int y2) const override;
    bool checkCastling(int x1, int y1, int x2, int y2) const;
    bool isEmpty() const override;
};



#endif


