#include <string>
#include <iostream>
#include "empty.h"
#include <vector>
using namespace std;

Empty::Empty(Board *board, PieceColour colour) : Chess{board, colour, true} { }

Empty::~Empty() {}

bool Empty::isEmpty() const { return this->empty; }

bool Empty::isLegalMove(int x1, int y1, int x2, int y2) const { return true; }

bool Empty::isBlocked(int x1, int y1, int x2, int y2) const { return false; }
