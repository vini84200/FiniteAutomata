//
// Created by vini84200 on 9/26/22.
//
#define DEBUG

#include "parser.h"
#include <string.h>
#include <iostream>
// Example of a file that can be parsed
//MeuAFD
//S: q0,q1,q2,q3
//A: a,b
//i: q0
//F: q2,q3
//
//(q0,a,q1)
//(q0,b,q3)
//(q1,a,q2)
//(q1,b,q1)
//(q2,a,q2)
//(q3,b,q2)


Parser::Parser() {}

Parser::~Parser() {}

Automato Parser::parseFile(std::string file_name) {
    // open file
    auto file = fopen(file_name.c_str(), "r");
    if (file == nullptr) {
        std::cout << "Arquivo não encontrado" << std::endl;
        exit(1);
    }
    // parse file content
    std::string name, initial_state;
    std::vector<std::string> states, finals;
    std::vector<char> alphabet;
    std::vector<std::tuple<std::string, char, std::string>> transitions;
    int stage = 0;
    while (!feof(file)) {
        // read line
        char line[100];
        fgets(line, 100, file);
        // remove \n
        line[strlen(line) - 1] = '\0';
        // check if line is empty
        if (strlen(line) == 0) {
            continue;
        }
        // parse line
        switch (stage) {
            case 0: {
                // read name
                name = line;

                stage++;
                break;
            }
            case 1: {
                // read states
                std::string states_str = line;
                int posCollon = states_str.find(":");
                states_str = states_str.substr(posCollon + 1);
                while (states_str.find(",") != std::string::npos) {
                    int posComma = states_str.find(",");
                    std::string state = states_str.substr(0, posComma);
                    // trim whitespaces
                    while (state[0] == ' ') {
                        state = state.substr(1);
                    }
                    while (state[state.length() - 1] == ' ') {
                        state = state.substr(0, state.length() - 1);
                    }
                    states.push_back(state);
                    states_str = states_str.substr(posComma + 1);
                }
                states.push_back(states_str);
                stage++;
                break;
            }
            case 2: {
                // read alphabet
                std::string alphabet_str = line;
                int posCollon = alphabet_str.find(":");
                alphabet_str = alphabet_str.substr(posCollon + 1);
                while (alphabet_str.find(",") != std::string::npos) {
                    int posComma = alphabet_str.find(",");
                    std::string symbol = alphabet_str.substr(0, posComma);
                    symbol = symbol.substr(symbol.find_first_not_of(' '));
                    symbol = symbol.substr(0, symbol.find_last_not_of(' ') + 1);
                    alphabet.push_back(symbol[0]);
                    alphabet_str = alphabet_str.substr(posComma + 1);
                }
                alphabet.push_back(alphabet_str[0]);
                stage++;
                break;
            }
            case 3: {
                // read initial state
                std::string initial_state_str = line;
                int posColon = initial_state_str.find(":");
                initial_state_str = initial_state_str.substr(posColon + 1);
                // trim whitespaces
                initial_state_str = initial_state_str.substr(initial_state_str.find_first_not_of(' '));
                initial_state_str = initial_state_str.substr(0, initial_state_str.find_last_not_of(' ') + 1);
                initial_state = initial_state_str;
                stage++;
                break;
            }
            case 4: {
                // read finals
                std::string finals_str = line;
                int posColon = finals_str.find(":");
                finals_str = finals_str.substr(posColon + 1);
                while (finals_str.find(",") != std::string::npos) {
                    int posComma = finals_str.find(",");
                    std::string final_state = finals_str.substr(0, posComma);
                    // trim whitespaces
                    final_state = final_state.substr(final_state.find_first_not_of(' '));
                    final_state = final_state.substr(0, final_state.find_last_not_of(' ') + 1);
                    finals.push_back(final_state);
                    finals_str = finals_str.substr(posComma + 1);
                }
                // trim whitespaces
                finals_str = finals_str.substr(finals_str.find_first_not_of(' '));
                finals_str = finals_str.substr(0, finals_str.find_last_not_of(' ') + 1);
                finals.push_back(finals_str);
                stage++;
                break;
            }
            case 5: {
                // Read transitions
                std::string transition_str = line;
                int posOpenParenthesis = transition_str.find("(");
                int posCloseParenthesis = transition_str.find(")");
                transition_str = transition_str.substr(posOpenParenthesis + 1,
                                                       posCloseParenthesis - posOpenParenthesis - 1);
                // Get state 1
                std::string state1 = transition_str.substr(0, transition_str.find(","));
                // trim whitespaces
                state1 = state1.substr(state1.find_first_not_of(' '));
                state1 = state1.substr(0, state1.find_last_not_of(' ') + 1);

                transition_str = transition_str.substr(transition_str.find(",") + 1);
                // Get symbol
                char symbol = transition_str.substr(0, transition_str.find(",")).c_str()[0];

                // Get state 2
                std::string state2 = transition_str;
                state2 = state2.substr(state2.find(",") + 1);
                // trim whitespaces
                state2 = state2.substr(state2.find_first_not_of(' '));
                state2 = state2.substr(0, state2.find_last_not_of(' ') + 1);

                // Add transition to list
                transitions.push_back(std::make_tuple(state1, symbol, state2));
                break;
            }
        }
    }
    // create automata
#ifdef DEBUG
    // Print automata
    printf("Name: %s \n", name.c_str());
    printf("States: \n");
    for (auto state: states) {
        printf("\t'%s' \n", state.c_str());
    }
    printf("Alphabet: \n");
    for (auto symbol: alphabet) {
        printf("\t'%c' \n", symbol);
    }
    printf("Initial state: '%s'\n", initial_state.c_str());
    printf("Final states: \n");
    for (auto final_state: finals) {
        printf("\t'%s' \n", final_state.c_str());
    }
    printf("Transitions: \n");
    for (auto transition: transitions) {
        printf("\t'%s' --%c-> '%s' \n", std::get<0>(transition).c_str(), std::get<1>(transition),
               std::get<2>(transition).c_str());
    }
    printf("\n");
#else
    printf("Automata %s created \n", name.c_str());
#endif
    Automato automata(name, alphabet, states, finals, initial_state);
    for (auto transition: transitions) {
        automata.createTransition(std::get<0>(transition), std::get<2>(transition), std::get<1>(transition));
    }
    fclose(file);
    // return automata
    return automata;
}
