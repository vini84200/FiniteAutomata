#include "automata.hpp"
#include <iostream>

int main() {
    std::vector<char> alphabet = {'a', 'b'};
    std::vector<std::string> states = {"q0", "q1", "q2"};
    std::vector<std::string> finals = {"q2"};
    std::string initial_state = "q0";
    Automato automato("Automato", alphabet, states, finals, initial_state);
    automato.isFinal("a");
    automato.isFinal("q2");
    automato.isFinal("q3");
    automato.createTransition("q0", "q1", 'a');
    automato.createTransition("q1", "q2", 'b');
    automato.createTransition("q2", "q1", 'a');
    printf("%d", automato.isAcepted("ab")); // 1
    printf("%d", automato.isAcepted("a")); // 0
    printf("%d", automato.isAcepted("aba")); // 0
    printf("%d", automato.isAcepted("abab")); // 1

}