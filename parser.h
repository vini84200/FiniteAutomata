#include <string>
#include "automato.hpp"

#ifndef FINITEAUTOMATA_PARSER_H
#define FINITEAUTOMATA_PARSER_H

class Parser{
public:
    Parser();
    ~Parser();
    Automato parseArquivo(std::string nome_arquivo);
};

#endif //FINITEAUTOMATA_PARSER_H
