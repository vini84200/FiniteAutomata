#include "automata.hpp"
#include <stdexcept>

Automato::Automato(std::string name, std::string alphabet,
                   std::string initial_state, std::string final_states,
                   std::string transitions) {
  std::string buffer0;
  this->name = name;
  for (int i = 0; i < alphabet.lenght(); i++) {
    if ((alphabet[i] != ' ') && (alphabet[i] != ',')) {
      this->alphabet.push_back(alphabet[i]);
    }
  }
  createState(initial_state);
  this->initial_state = getStateId(initial_state);
  for (int i = 0; i < final_states.lenght(); i++) {
    if ((final_states[i] != ' ') && (final_states[i] != ',')) {
      this->final_states.push_back(std::stoi(final_states[i]));
    }
  }
  last_id = -1;
}

Automato::~Automato() {}

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

void Automato::createState(std::string name) {
  int id = ++this->last_id;
  this->q_to_id[name] = id;
  this->transitions[id] = std::map<char, int>();
}

void Automato::createTransition(std::string q1, std::string q2, char c) {
  int s1 = this->getStateId(q1);
  int s2 = this->getStateId(q2);
  if (s1 == -1 || s2 == -1) {
    return;
  }
  
  this->transitions[s1][c] = s2;
}

void readTransitions(std::string str){
  std::string buffer;
  buffer = 
}