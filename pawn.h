#ifndef _PAWN_H_
#define _PAWN_H_
#include "chess.h"

class Board;

class Pawn : public Chess {
    public:
        Pawn(Board* board, PieceColour colour);
        ~Pawn();

        bool isEmpty() const override;
        bool isLegalMove(int x1, int y1, int x2, int y2) const override;
        bool isBlocked(int x1, int y1, int x2, int y2) const override;

        bool isCapture(int x1, int y1, int x2, int y2) const;
        bool isFirstMove() const;
        bool isEndOfBoard() const;
        bool isEnPassant(int x1, int y1, int x2, int y2) const;
};

#endif
