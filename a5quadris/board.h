/*
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/

#ifndef __BOARD_H__
#define __BOARD_H__

//#include "window.h"
#include <iostream>
//Remove for release version
#include "block.h"

const int boardH = 18; //Height of board
const int boardW = 10; //Width of board
const int maxLevel = 4; //Maximum number of levels

class Level; //Forward-declare levels

class Board {
  Block* theBoard[boardH][boardW]; //The actual board
  Block* current; //Pointer to curretn block
  Block* next; //Pointer to the next block
  Level* lvls[maxLevel]; //Array of level objects
  int curr[4][2]; //Position of the current block
  int shadow[4][2]; //Position of the shadow
  int currentLevel; //Current level
  void eraseCell(int r, int c, bool scoring); //Erases a given coordinate
  void eraseRow(int i, bool scoring); //Erases a row
  void shiftDown(int r); //Shifts down from a row
  void updateShadow(); //Updates the shadow
  bool blockFits(Block* b); //Checks that a block fits in the board
  int currentShape[4][4]; //Shape of the current block
  int score; //Current score
  int highScore; //High score
public:
  void newGame(int levelNum); //Creates a new game
  Board(std::string file, int levelNum); 
    //file is what file Level 0 reads from, levelNum is starting level
  ~Board();
  void changeLevel(int n); //Changes the current level
  int getLevel(); //Gets the current level
  char getBlockAt(int r, int c); //Gets the type of the block at given position
  int getShadowPos(int i, int c); //Gets position of the shadow
  char getNext(int r, int c); //Gets the shape of the next block
  void rotate(int dir); //Rotates the current block
  void move(int dir); //Moves the current block
  bool check(); //Checks for scoring and generates a new block
  int getScore(); //Gets current score
  int getHS(); //Gets high score

  //friend std::ostream &operator<<(std::ostream &out, Board &b);
  //Remove for release version
};

#endif
