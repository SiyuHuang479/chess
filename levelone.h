#ifndef _LEVELONE_H_
#define _LEVELONE_H_
#include <iostream>
#include <string>
#include "computer.h"

class LevelOne: public Computer {

 public:
    LevelOne(string name, PieceColour colour, Board* board);
    bool makeMove() override;
};

#endif
