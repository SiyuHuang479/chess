#include <string>
#include "player.h"
using namespace std;

Player::~Player() { }

Player::Player(string name, PieceColour colour, Board* board): 
    name{name}, board{board}, colour{colour}, score{0} { }

string Player::getName() const { return name; }

int Player::getScore() const { return score; }

PieceColour Player::getColour() const { return colour; }

void Player::setScore(int new_score) { score = new_score; }
