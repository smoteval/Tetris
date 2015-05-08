/* 
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/  


#include <cstdlib>
#include <iostream>
#include <ios>
#include <fstream>  
#include "level.h"
#include "block.h"  

// Constructor 
Level::Level(const int p[7], std::string file) {
  if (file != "") input = new std::ifstream(file.c_str());
  else input = NULL;
  for (int i=0;i<7;i++) {
    probs[i]=p[i];
  }
}

// destructor
Level::~Level() {
  delete input;
}

//Method:
Block *Level::createBlock() {
  int ran = rand() % 100; // this is a randomly generated integer that will be used later 
  // for levels > 0:
  if (input == NULL) {
    int sum =0; //The sum of the integers in our array prob 
    for (int i=0;i<7;i++) sum = sum + probs[i];
    int probability[7]; // this array will be filled with the probability of our blocks
    for (int i =0;i<7;i++) {
      probability[i] = (100*probs[i])/sum ;
    }
    //int probabilityArray[100];


    //Now we will call the ctor of block choosing the type of the block based on the probability assigned in the level
    int current = probability[0];
    if (ran <= current) return new Block('I');
    current = current + probability[1];
    if (ran <= current) return new Block('J');
    current = current + probability[2];
    if (ran <= current) return new Block('L');
    current = current + probability[3];
    if (ran <= current) return new Block('O');
    current = current + probability[4];
    if (ran <= current) return new Block('S');
    current = current + probability[5];
    if (ran <= current) return new Block('T'); 
    return new Block('Z');
  }
  else {
    char a; // this char is the type of the block that will be read from the file
    *input >> std::skipws >> a;
    if (input->eof()) {
      input->clear();
      input->seekg(0, std::ios::beg);
      *input >> std::skipws >> a;
    }
    return new Block(a);
  }
}  
