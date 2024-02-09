#include <iostream>
#include <string>
#include "textdisplay.h"
using namespace std;

TextDisplay::TextDisplay() : theDisplay(boardSize, vector<char>(boardSize, '_')) { 
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) theDisplay[i][j] = ' ';
    }
  }
}


char charDescription(Chess& c) {

  string description = c.returnDescription();
  if (c.isEmpty() || c.returnDescription() == "empty") {
    if (c.getX() % 2 == 0 && c.getY() % 2 == 0) return ' ';
    if (c.getX() % 2 == 1 && c.getY() % 2 == 1) return ' ';
    return '_';
  }

  if (c.returnColour() == PieceColour::Black) {
    if (description == "rook") return 'r';
    if (description == "bishop") return 'b';
    if (description == "knight") return 'n';
    if (description == "queen") return 'q';
    if (description == "king") return 'k';
    if (description == "pawn") return 'p';
  }

  if (description == "rook") return 'R';
  if (description == "bishop") return 'B';
  if (description == "knight") return 'N';
  if (description == "queen") return 'Q';
  if (description == "king") return 'K';
  if (description == "pawn") return 'P';
  if (c.getX() % 2 == 0 && c.getY() % 2 == 0) return ' ';
  if (c.getX() % 2 == 1 && c.getY() % 2 == 1) return ' ';
  return '_';
}



void TextDisplay::notify(Chess &c, int x1, int y1, int x2, int y2) {
  if ((x1 % 2 == 0 && y1 % 2 == 0) || (x1 % 2 == 1 && y1 % 2 == 1)) {
    theDisplay[x1][y1] = ' ';
  } else {
    theDisplay[x1][y1] = '_';
  }
  theDisplay[x2][y2] = charDescription(c);
}

TextDisplay::~TextDisplay() { 
  theDisplay.clear();
}


ostream &operator<<(ostream &out, const TextDisplay &td) {
  for (int i = 0; i < boardSize; ++i) {
    out << 8 - i << " ";
    for (int k = 0; k < boardSize; ++k) {
      out << td.theDisplay[i][k];
    }
    out << endl;
  }
  out << endl << "  abcdefgh" << endl;
  return out;
}
