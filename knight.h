#ifndef __KNIGHT_H__
#define __KNIGHT_H__
#include "chess.h"

class Board;

class Knight: public Chess {
 public:
    Knight(Board* board, PieceColour colour);
    bool isEmpty() const;
    bool isLegalMove(int x1, int y1, int x2, int y2) const override;
    bool isBlocked(int x1, int y1, int x2, int y2) const override;
};

#endif
