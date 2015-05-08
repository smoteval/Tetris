/*
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/



#include <exception>
#include <iostream>
#include "block.h"

//Constructor
// Note: our convention for creating a block is that the top left most of our 4*4 matrix will be filled according to the type of the block
Block::Block(const char &type): level(-1) {
  // creating I block
  if (type == 'I') {
    for (int i=0;i<4;i++) {
      shape[0][i]=1;
    for (int j=1;j<4;j++) shape[j][i]=0;
    }
  }

  //Creating J Block
  else if (type == 'J') {
    //filling the matrix for the Block
    shape[0][0] = 1;
    shape[1][0] = 1;
    shape[1][1] = 1;
    shape[1][2] = 1;
    //filling zeroes on the rest of the matrix
    shape[1][3] = 0;
    for (int i=1;i<4;i++) shape[0][i]=0;
    for (int i=0;i<4;i++) shape[2][i]=0;
    for (int i=0;i<4;i++) shape[3][i]=0;
  }

  //Creating O Block
  else if (type == 'O') {
    //filling the matrix for the O Block:
    shape[0][0] = 1;
    shape[0][1] = 1;
    shape[1][0] = 1;
    shape[1][1] = 1;
    //Filling zeroes on the rest of the matrix:
    for (int i=0;i<4;i++) shape[i][2]=0;
    for (int i=0;i<4;i++) shape[i][3]=0;
    for (int i=2;i<4;i++) shape[i][0]=0;
    for (int i=2;i<4;i++) shape[i][1]=0;
  }

  //Creating L Block:
  else if (type == 'L') {
    //filling the matrix for L Block:
    shape[0][2] = 1;
    for (int i =0;i<3;i++) shape[1][i] = 1;
    //Filling zeroes in the rest of the matrix:
    shape[0][0]=0;
    shape[0][1]=0;
    shape[1][3]=0;
    for (int i=0;i<4;i++) shape[2][i]=0;
    for (int i=0;i<4;i++) shape[3][i]=0;
  }

  //Creating the S Block:
  else if (type == 'S') {
    //filling the matrix for the S Block
    shape[0][1] = 1;
    shape[0][2] = 1;
    shape[1][0] = 1;
    shape[1][1] = 1;
    //Filling the rest of the Matrix with zeroes:
    shape[0][0]=0;
    shape[0][3]=0;
    shape[1][2]=0;
    shape[1][3]=0;
    for (int i=0;i<4;i++) shape[2][i]=0;
    for (int i=0;i<4;i++) shape[3][i]=0;
  }
  //Creating Z Block;
  else if (type == 'Z') {
    //Filling the matrix for the Z block:
    shape[0][0] = 1;
    shape[0][1] = 1;
    shape[1][1] = 1;
    shape[1][2] = 1;
    //Filling the rest of the matrix with zeroes:
    shape[0][2]=0;
    shape[0][3]=0;
    shape[1][0]=0;
    shape[1][3]=0;
    for (int i=0;i<4;i++) shape[2][i]=0;
    for (int i=0;i<4;i++) shape[3][i]=0;
  }

  //Creating T Block:
  else if (type == 'T') {
    //Filling the matrix for The T Block:
    for(int i=0;i<3;i++) shape[0][i] = 1;
    shape[1][1] = 1;
    //Filling the rest of the matrix with zeroes:
    shape[0][3]=0;
    shape[1][0]=0;
    shape[1][2]=0;
    shape[1][3]=0;
    for (int i=0;i<4;i++) shape[2][i]=0;
    for (int i=0;i<4;i++) shape[3][i]=0;
  }
  // if the given char is not any known type of block:
  else {
    std::cout << "fix it later" << std::endl;
  }

  // other fields
  this->type = type;
  this->count = 4;
}
