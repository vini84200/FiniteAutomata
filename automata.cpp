#include "automata.hpp"
#include <stdexcept>

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
    return isFinalInt(q);
}

void Automato::createState(std::string name) {
    if (this->getStateId(name) != -1) {
        return;
    }
    int id = ++this->last_id;
    this->q_to_id[name] = id;
    this->transitions.emplace_back();
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

Automato::Automato(std::string name, const std::vector<char> &alphabet,
                   std::vector<std::string> states, std::vector<std::string> finals,
                   std::string initial_state) {

    this->name = name;
    for (char i: alphabet) {
        this->alphabet.push_back(i);
    }

    last_id = -1;
    createState(initial_state);
    this->initial_state = getStateId(initial_state);

    for (auto &final: finals) {
        createState(final);
        this->finals.push_back(getStateId(final));
    }

    for (auto &state: states) {
        createState(state);
    }

}