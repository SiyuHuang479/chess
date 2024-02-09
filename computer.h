#ifndef _COMPUTER_H_
#define _COMPUTER_H_
#include <iostream>
#include <string>
#include "player.h"

using namespace std;

enum class Level { Level1, Level2, Level3, Level4 };

class Computer: public Player {
    Level level;

 protected:
    bool isPromotion(int x1, int y1, int x2, int y2);
    bool isPawnCapture(int x1, int y1, int x2, int y2);

 public:
    Computer(string name, PieceColour colour, Board* board, Level level);

    // getter and setter
    int getLevel();
    void setLevel(int levelNum);

    virtual bool makeMove() = 0; //???
};

#endif
