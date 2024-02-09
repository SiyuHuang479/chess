#ifndef _HUMAN_H_
#define _HUMAN_H_
#include <iostream>
#include <string>
#include "player.h"

using namespace std;

struct eof { };

class Human: public Player {

 public:
    Human(string name, PieceColour colour, Board* board);
    bool makeMove() override;
};

#endif
