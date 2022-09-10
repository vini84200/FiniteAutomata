#include <map>
#include <vector>
#include <string>

//TODO: Automato Builder

class Automato {
private:
  std::string name;
  std::vector<char> alphabet;
  // Transições
  std::map<std::string, int> q_to_id;
  std::vector<std::map<char, int>> transitions;
  std::vector<int> finals;
  int initial_state;

  int last_id;

  int getStateId(std::string s);
  int nextState(int current_s, char read);
  bool isFinalInt(int q);
public:
  ~Automato();
  Automato(std::string name,
           const std::vector<char>& alphabet,
           std::vector<std::string> states,
           std::vector<std::string> finals,
           std::string initial_state);

  bool isFinal(std::string s);
  bool isAcepted(std::string word);

  void createState(std::string name);
  void createTransition(std::string q1, std::string q2, char c);
  //TODO: Constructor
};