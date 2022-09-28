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

#endif //FINITEAUTOMATA_PARSER_H
