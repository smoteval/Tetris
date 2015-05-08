/* 
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/  

// This class is based on The trie class that was provided in Assignment 3 
// and being used in quadris for the purpose of autocompleting commands  

#ifndef __TRIE_H__
#define __TRIE_H__

#include <string>

const int NumChars = 26;

class TrieNode {
  bool isWord;
  TrieNode *letters[NumChars];
public:
  TrieNode();
  ~TrieNode();

  void insert(const std::string &word);

  std::string print(const std::string &word);    
  std::string find(const std::string &word);
  std::string autocomplete(const std::string &word);
}; 


#endif
