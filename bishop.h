#ifndef _BISHOP_H_
#define _BISHOP_H_
#include "chess.h"

class Board;

class Bishop : public Chess {
    public:
        Bishop(Board* board, PieceColour colour);

        bool isEmpty() const override; // Returns true if the current chess piece is the empty piece

        bool isLegalMove(int x1, int y1, int x2, int y2) const override;

        bool isBlocked(int x1, int y1, int x2, int y2) const override;
};

#endif
