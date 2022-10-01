#define DEBUG

#include "parser.h"
#include <string.h>
#include <iostream>

Parser::Parser(){

}

Parser::~Parser(){

}

Automato Parser::parseArquivo(std::string nome_arquivo){
    // Abre arquivo para leitura
    auto arq = fopen(nome_arquivo.c_str(), "r");
    if(arq == nullptr){
        std::cout << "Arquivo não encontrado" << std::endl;
        exit(1);
    }
    // Processa o arquivo
    std::string nome, estado_inicial;
    std::vector<std::string> estados, finais;
    std::vector<char> alfabeto;
    std::vector<std::tuple<std::string, char, std::string>> transicoes;
    int etapa = 0;
    while(!feof(arq)){
        // read linha
        char linha[100];
        fgets(linha, 100, arq);
        // Remove CRLF
        linha[strlen(linha) - 1] = '\0';
        if(linha[strlen(linha) - 1] == '\r'){
            linha[strlen(linha) - 1] = '\0';
        }
        // Verifica se a linha está vazia
        if(strlen(linha) == 0){
            continue;
        }
        // Parse linha
        switch(etapa){
            // Lê nome
            case 0:{
                nome = linha;
                etapa++;
                break;
            }
            // Lê estados
            case 1:{
                std::string string_estados = linha;
                int posDoisPontos = string_estados.find(":");
                string_estados = string_estados.substr(posDoisPontos + 1);
                while(string_estados.find(",") != std::string::npos){
                    int posVirgula = string_estados.find(",");
                    std::string estado = string_estados.substr(0, posVirgula);
                    // Corta espaços vazios
                    while(estado[0] == ' '){
                        estado = estado.substr(1);
                    }
                    while(estado[estado.length() - 1] == ' '){
                        estado = estado.substr(0, estado.length() - 1);
                    }
                    estados.push_back(estado);
                    string_estados = string_estados.substr(posVirgula + 1);
                }
                estados.push_back(string_estados);
                etapa++;
                break;
            }
            // Lê alfabeto
            case 2:{
                std::string string_alfabeto = linha;
                int posDoisPontos = string_alfabeto.find(":");
                string_alfabeto = string_alfabeto.substr(posDoisPontos + 1);
                while (string_alfabeto.find(",") != std::string::npos) {
                    int posVirgula = string_alfabeto.find(",");
                    std::string simbolo = string_alfabeto.substr(0, posVirgula);
                    simbolo = simbolo.substr(simbolo.find_first_not_of(' '));
                    simbolo = simbolo.substr(0, simbolo.find_last_not_of(' ') + 1);
                    alfabeto.push_back(simbolo[0]);
                    string_alfabeto = string_alfabeto.substr(posVirgula + 1);
                }
                alfabeto.push_back(string_alfabeto[0]);
                etapa++;
                break;
            }
            // Lê estado inicial
            case 3:{
                std::string string_estado_inicial = linha;
                int posColon = string_estado_inicial.find(":");
                string_estado_inicial = string_estado_inicial.substr(posColon + 1);
                // Corta espaços vazios
                string_estado_inicial = string_estado_inicial.substr(string_estado_inicial.find_first_not_of(' '));
                string_estado_inicial = string_estado_inicial.substr(0, string_estado_inicial.find_last_not_of(' ') + 1);
                estado_inicial = string_estado_inicial;
                etapa++;
                break;
            }
            // Lê finais
            case 4:{
                std::string string_finais = linha;
                int posDoisPontos = string_finais.find(":");
                string_finais = string_finais.substr(posDoisPontos + 1);
                while(string_finais.find(",") != std::string::npos){
                    int posVirgula = string_finais.find(",");
                    std::string estado_final = string_finais.substr(0, posVirgula);
                    // Corta espaços vazios
                    estado_final = estado_final.substr(estado_final.find_first_not_of(' '));
                    estado_final = estado_final.substr(0, estado_final.find_last_not_of(' ') + 1);
                    finais.push_back(estado_final);
                    string_finais = string_finais.substr(posVirgula + 1);
                }
                // Corta espaços vazios
                string_finais = string_finais.substr(string_finais.find_first_not_of(' '));
                string_finais = string_finais.substr(0, string_finais.find_last_not_of(' ') + 1);
                finais.push_back(string_finais);
                etapa++;
                break;
            }
            // Lê transições
            case 5:{
                std::string string_transicoes = linha;
                int posAbreParenteses = string_transicoes.find("(");
                int posFechaParenteses = string_transicoes.find(")");
                string_transicoes = string_transicoes.substr(posAbreParenteses + 1,
                                                             posFechaParenteses - posAbreParenteses - 1);
                // Pega estado de saída da transição
                std::string estado1 = string_transicoes.substr(0, string_transicoes.find(","));
                // Corta espaços vazios
                estado1 = estado1.substr(estado1.find_first_not_of(' '));
                estado1 = estado1.substr(0, estado1.find_last_not_of(' ') + 1);
                string_transicoes = string_transicoes.substr(string_transicoes.find(",") + 1);
                // Pega símbolo
                char simbolo = string_transicoes.substr(0, string_transicoes.find(",")).c_str()[0];
                // Pega estado de chegada da transição
                std::string estado2 = string_transicoes;
                estado2 = estado2.substr(estado2.find(",") + 1);
                // Corta espaços vazios
                estado2 = estado2.substr(estado2.find_first_not_of(' '));
                estado2 = estado2.substr(0, estado2.find_last_not_of(' ') + 1);
                // Adiciona transição na lista
                transicoes.push_back(std::make_tuple(estado1, simbolo, estado2));
                break;
            }
        }
    }
    // Cria autômato
#ifdef DEBUG
    // Imprime autômato
    printf("Nome: %s \n", nome.c_str());
    printf("Estados: \n");
    for(auto estado: estados){
        printf("\t'%s' \n", estado.c_str());
    }
    printf("Alfabeto: \n");
    for(auto simbolo: alfabeto){
        printf("\t'%c' \n", simbolo);
    }
    printf("Estado inicial: '%s'\n", estado_inicial.c_str());
    printf("Estados finais: \n");
    for(auto estado_final: finais){
        printf("\t'%s' \n", estado_final.c_str());
    }
    printf("Transicoes: \n");
    for(auto transicao: transicoes){
        printf("\t'%s' --%c-> '%s' \n", std::get<0>(transicao).c_str(), std::get<1>(transicao),
               std::get<2>(transicao).c_str());
    }
    printf("\n");
#else
    printf("Automato %s criado \n", nome.c_str());
#endif
    Automato automato(nome, alfabeto, estados, finais, estado_inicial);
    for(auto transicao: transicoes){
        automato.criaTransicao(std::get<0>(transicao), std::get<2>(transicao), std::get<1>(transicao));
    }
    fclose(arq);
    // Retorna autômato
    return automato;
}
