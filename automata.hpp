#include <map>
#include <vector>
#include <string>

#ifndef AUTOMATA_HPP
#define AUTOMATA_HPP

class Automato {
private:
    std::string name;
    std::vector<char> alphabet;
    // Transições
    std::map<std::string, int> q_to_id;
    std::map<int, std::string> id_to_q;
    std::map<int, std::map<char, int>> transitions;
    std::vector<int> finals;
    int initial_state;

    int last_id;

    int getStateId(const std::string &s);

    int nextState(int current_s, char read);

    bool isFinalInt(int q);

public:
    Automato(const std::string &name,
             const std::vector<char> &alphabet,
             const std::vector<std::string> &states,
             const std::vector<std::string> &finals,
             const std::string &initial_state);

    bool isFinal(const std::string& a);

    bool isAccepted(const std::string& word);

    void createState(const std::string& state_name);

    void createTransition(const std::string &q1, const std::string &q2, char c);

    void print();

    void removeState(const std::string &state);

    void removeState(int state);

    void merge_states(const std::string &state1, const std::string &state2);

    void simplify();

    std::string getStateName(int i);

    void removeUnreachableStates();

    void removeDeadStates();

    void mergeEquivalentStates();
};

#endif