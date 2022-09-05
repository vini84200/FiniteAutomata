#include <map>
#include <vector>
#include <string>

//TODO: Automato Builder

class Automato {
private:
  // Transições
  std::map<std::string, int> q_to_id;
  std::vector<std::map<char, int>> transitions;
  std::vector<int> finals;
  int initial_state;

  int getStateId(std::string s);
  int nextState(int current_s, char read);
  bool isFinalInt(int q);
public:
  bool isFinal(std::string s);
  bool isAcepted(std::string word);

  //TODO: Constructor
};