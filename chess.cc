#include "chess.h"
#include "board.h"
using namespace std;

Chess::Chess(Board* board, PieceColour colour, bool empty) : 
  board{board}, colour{colour}, numberOfMoves{0}, empty{empty} {}

int Chess::getX() const { return x; }
int Chess::getY() const { return y; }

void Chess::setCoords(int r, int c) { this->x = r; this->y = c; }

bool Chess::isEmpty() const { return empty; }

void Chess::setEmpty(bool b) { 
  empty = b;
  // if (b) colour = PieceColour::Empty; 
}

void Chess::setColor(PieceColour p) { colour = p; }

void Chess::attach(Observer *o) { 
  observers.emplace_back(o);
}

void Chess::detach(Observer *o) {
    observers.pop_back();
}

void Chess::notifyObservers(int x1, int y1, int x2, int y2) {
  for (auto ob : observers) {
    ob->notify(*this, x1, y1, x2, y2);
  }
}

int Chess::getMoveNum() const {
    return numberOfMoves;
}

void Chess::addMoveNum() {
    numberOfMoves++;
}

string Chess::returnDescription() const {
  return description;
}

PieceColour Chess::returnColour() const {
  return colour;
}

void Chess::setDescription(std::string description) { this->description = description; }

Chess::~Chess() { }
