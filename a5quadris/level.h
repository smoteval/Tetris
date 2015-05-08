/* 
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/  

// The Purpose of this class is to store the data relevant to Level (probabilities and ...)
// and creating a block according to the level  

#ifndef _LEVEL_
#define _LEVEL_

#include <iostream>
#include "block.h"

class Level {
  int probs[7]; // this array hold integers that can determine the probability of our 7 blocks
  std::istream *input; // we need this field because at level 0 we are getting the blocks from a file
public:
  Level(const int p[7], std::string file);
  ~Level();
  Block *createBlock();
};

#endif
