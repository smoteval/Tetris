/* 
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/  
  
#include "trie.h"

TrieNode::TrieNode() {
  isWord = false;
  for (int i = 0; i < NumChars; ++i)
    letters[i] = 0;
  //Setting up root
}

TrieNode::~TrieNode() {
  for (int i = 0; i < NumChars; ++i)
    delete letters[i];
}

void TrieNode::insert(const std::string &word) {
  if (word != "") {
    int temp = word.c_str()[0] - 'a';
    if (!letters[temp])
      letters[temp] = new TrieNode();
    letters[temp]->insert(word.substr(1));
    //Recursively insert, shortening word
  }
  else
    isWord = true;
    //Mark as a word when parameter is empty
}

std::string TrieNode::print(const std::string &word) {
//This helper uses an accumulator to find all suggestions
//Should be called with the prefix, or it will not
//print the full suggestions
  std::string temp = "";
  for (int i = 0; i < NumChars; ++i) {
    if (letters[i]) {
      char letter = (char) i + 'a';
      temp = temp + letters[i]->print(word + letter);
    }
    //Recursively generate all words
  }
  return (isWord ? (word + " ") : "") + temp;
}

std::string TrieNode::find(const std::string &word) {
  std::string temp = "";
  TrieNode *current = this;
  for (unsigned int i = 0; i < word.length(); ++i) {
    if (word.c_str()[i] < 97 || word.c_str()[i] > 122)
      return " ";
    if (current)
      current = current->letters[word.c_str()[i] - 'a'];
  }
  //Find prefix in trie, if it exists
  if (current) {
    temp = current->print(word);
    if (temp == "") { //If no suggestions
      for (int i = 0; i < NumChars; ++i) {
        delete current->letters[i];
        current->letters[i] = 0;
        //Delete nodes and set pointers to 0
      }
    }
  }
  return std::string(temp).substr(0, temp.length() - 1);
  //'print' adds an extra space at he end, must be removed
}

std::string TrieNode::autocomplete(const std::string &word) {
  std::string temp = find(word);
  return (temp.find(" ") == std::string::npos) ? temp : "";
}
