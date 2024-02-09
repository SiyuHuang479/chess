#include <string>
#include "computer.h"
using namespace std;

Computer::Computer(string name, PieceColour colour, Board* board, Level level): 
    Player{name, colour, board}, level{level} {}

bool Computer::isPromotion(int x1, int y1, int x2, int y2) {
    if (board->returnChess(x1, y1)->returnDescription() == "pawn" &&
        ((board->returnChess(x1, y1)->returnColour() == PieceColour::Black && x2 == 7) || 
        (board->returnChess(x1, y1)->returnColour() == PieceColour::White && x2 == 0))) {
        return true;
    }
    return false;
}

bool Computer::isPawnCapture(int x1, int y1, int x2, int y2) {

        if (board->returnChess(x1, y1)->returnColour() == PieceColour::White) {
            // check white capture
            if (abs(y2 - y1) == 1 && (x2 - x1) == -1 && !board->isEmpty(x2, y2)) {
                if (board->returnChess(x2, y2)->returnDescription() == "king") return false;
                return true;
            }
            // check white en passant
            else if (abs(y2 - y1) == 1 && x2 - x1 == -1 &&
                    !board->returnChess(x2 + 1, y2)->isEmpty() && 
                    board->returnChess(x2 + 1, y2)->returnDescription() == "pawn" &&
                    board->returnChess(x2 + 1, y2)->getMoveNum() == 1 &&
                    x2 == board->getLastMove().at(1).at(0) - 1 &&
                    y2 == board->getLastMove().at(1).at(1)) {
                return true;
            } else return false; // no capturing move
        } else {
            // check black capture
            if (abs(y2 - y1) == 1 && (x2 - x1) == 1 && !board->isEmpty(x2, y2)) {
                if (board->returnChess(x2, y2)->returnDescription() == "king") return false;
                return true;
            }
            // check black en passant
            else if (abs(y2 - y1) == 1 && x2 - x1 == 1 &&
                    !board->returnChess(x2 - 1, y2)->isEmpty() &&
                    board->returnChess(x2 - 1, y2)->returnDescription() == "pawn" &&
                    board->returnChess(x2 - 1, y2)->getMoveNum() == 1 &&
                    x2 == board->getLastMove().at(1).at(0) + 1 &&
                    y2 == board->getLastMove().at(1).at(1)) {
                return true;
            } else return false; // no capturing move
        }
}

int Computer::getLevel() {
    if (level == Level::Level1) return 1;
    else if (level == Level::Level2) return 2;
    else return 3;
}

void Computer::setLevel(int levelNum) {
    level = Level(levelNum - 1);
}
