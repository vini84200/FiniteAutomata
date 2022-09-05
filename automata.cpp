#include "automata.hpp"
#include <stdexcept>

bool Automato::isFinal(std::string a) {
  int id = this->getStateId(a);
  int s;
  for (int s : this->finals) {
    if (s == id)
      return true;
  }
  return false;
};

bool Automato::isFinalInt(int a) {
  for (int s : this->finals) {
    if (s == a)
      return true;
  }
  return false;
};

int Automato::getStateId(std::string s) {
  try {
    return this->q_to_id.at(s);
  } catch (std::out_of_range ex) {
    return -1;
  }
}

int Automato::nextState(int current_s, char read) {
  try {
    return this->transitions.at(current_s).at(read);
  } catch (std::out_of_range ex) {
    return -1;
  }
}

bool Automato::isAcepted(std::string word) {
  int q = this->initial_state;
    
  for (char c : word) {
    if (q == -1) {
      return isFinalInt(q);
    }
    q = nextState(q, c);
  }
  
}
