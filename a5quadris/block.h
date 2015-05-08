/*
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/


// The block class is for storing the required data for each block and
// since we don't have any private fields or methods, we are using struct instead  


#ifndef _BLOCK_
#define _BLOCK_

struct Block {

char type;
int shape[4][4];
int count;
int level;

Block(const char &type);

};

#endif
