#define DEBUG ded
#include "automata.hpp"
#include "parser.h"

int main() {
    Parser parser;
    Automato automato = parser.parseFile("automato.txt");
    printf("Accepts 'abba'? %s\n", automato.isAccepted("abba") ? "true" : "false");
    printf("Accepts 'a'? %s\n", automato.isAccepted("a") ? "true" : "false");
    printf("Accepts 'acgim'? %s \n", automato.isAccepted("acgim") ? "true" : "false");
    printf("Accepts 'abgklko'? %s \n", automato.isAccepted("abgklko") ? "true" : "false");
    printf("Accepts ''? %s \n", automato.isAccepted("") ? "true" : "false");
}
