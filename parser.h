//
// Created by vini84200 on 9/26/22.
//
#include <string>
#include "automata.hpp"
#ifndef FINITEAUTOMATA_PARSER_H
#define FINITEAUTOMATA_PARSER_H


class Parser {
    // Use builder pattern to create a parser
    // that can parse a file and create a finite automata
public:
    Parser();
    ~Parser();
    Automato parseFile(std::string file_name);
};


class Transition {
public:
    std::string state1;
    std::string state2;
    char symbol;
    Transition(std::string state1, std::string state2, char symbol) {
        this->state1 = state1;
        this->state2 = state2;
        this->symbol = symbol;
    }
};

#endif //FINITEAUTOMATA_PARSER_H
