#include "automato.hpp"
#include <stdexcept>
#include <algorithm>
#include <iostream>

bool Automato::ehFinal(const std::string& a){
    int id = this->getIdEstado(a);
    return std::any_of(this->finais.begin(), this->finais.end(), [id](int s) { return s == id; });
}

bool Automato::ehIntFinal(int a){
    return std::any_of(this->finais.begin(), this->finais.end(), [a](int s) { return s == a; });
}

int Automato::getIdEstado(const std::string &s){
    try{
        return this->q_para_id.at(s);
    }catch(std::out_of_range &ex){
        return -1;
    }
}

int Automato::proxEstado(int string_atual, char lido) {
    try{
        return this->transicoes.at(string_atual).at(lido);
    }catch (std::out_of_range &ex){
        return -1;
    }
}

bool Automato::ehAceita(const std::string& palavra){
    int q = this->estado_inicial;
    for(char c: palavra){
        if(q == -1){
            return ehIntFinal(q);
        }
        q = proxEstado(q, c);
    }
    return ehIntFinal(q);
}

void Automato::criaEstado(const std::string& nome_estado){
    if(this->getIdEstado(nome_estado) != -1){
        return;
    }
    int id = ++this->ultimo_id;
    this->q_para_id[nome_estado] = id;
    this->id_para_q[id] = nome_estado;
    this->transicoes[id] = std::map<char, int>();
    for(char c: this->alfabeto){
        this->transicoes[id][c] = -1;
    }
}

void Automato::criaTransicao(const std::string &q1, const std::string &q2, char c){
    int s1 = this->getIdEstado(q1);
    int s2 = this->getIdEstado(q2);
    if(s1 == -1 || s2 == -1){
        return;
    }
    this->transicoes[s1][c] = s2;
}

Automato::Automato(const std::string &_nome, const std::vector<char> &_alfabeto,
                   const std::vector<std::string> &estados, const std::vector<std::string> &_finais,
                   const std::string &_estado_inicial){
    this->nome = _nome;
    for(char i: _alfabeto){
        this->alfabeto.push_back(i);
    }
    ultimo_id = -1;
    criaEstado(_estado_inicial);
    this->estado_inicial = getIdEstado(_estado_inicial);
    for(auto &final: _finais){
        criaEstado(final);
        this->finais.push_back(getIdEstado(final));
    }
    for(auto &estado: estados){
        criaEstado(estado);
    }
}

void Automato::imprime(){
    std::cout << "Automato " << nome << std::endl;
    std::cout << "Alfabeto: ";
    for(auto &simbolo: alfabeto){
        std::cout << simbolo << " ";
    }
    std::cout << std::endl;
    std::cout << "Estados: ";
    for(auto &estado: id_para_q){
        std::cout << estado.second << " ";
    }
    std::cout << std::endl;
    std::cout << "Estado inicial: " << getNomeEstado(estado_inicial) << std::endl;
    std::cout << "Estados finais: ";
    for(auto &final: finais){
        std::cout << getNomeEstado(final) << " ";
    }
    std::cout << std::endl;
    std::cout << "Transicoes: " << std::endl;
    for(unsigned int i = 0; i < transicoes.size(); i++){
        for(auto &transicao: transicoes[i]){
            if(transicao.second != -1){
                std::cout << getNomeEstado(i) << " -> " << getNomeEstado(transicao.second) << " : " << transicao.first
                          << std::endl;
            }
        }
    }
    std::cout << std::endl;
}

void Automato::removeEstado(const std::string &estado) {
    printf("removendo estado %s\n", estado.c_str());
    int id_estado = getIdEstado(estado);
    if(id_estado == -1){
        return;
    }
    // Remove de id_para_q
    id_para_q.erase(id_estado);
    // Remove de q_para_id
    q_para_id.erase(estado);
    // Remove de finais
    this->finais.erase(std::remove(this->finais.begin(), this->finais.end(), this->getIdEstado(estado)),
                       this->finais.end());
    // Remove de transicoes
    for(unsigned int i = 0; i < this->transicoes.size(); i++){
        if(i != (unsigned int) id_estado){
            std::vector<char> remover;
            for(auto &transicao: this->transicoes[i]){
                if(transicao.second == id_estado){
                    remover.push_back(transicao.first);
                }
            }
            for(auto &c: remover){
                this->transicoes[i].erase(c);
            }
        }
    }
    this->transicoes.erase(id_estado);
}

void Automato::uneEstados(const std::string &estado1, const std::string &estado2) {
    printf("unindo estados %s e %s\n", estado1.c_str(), estado2.c_str());
    int id_estado1 = getIdEstado(estado1);
    int id_estado2 = getIdEstado(estado2);
    // Remove o estado do vetor de estados
    this->q_para_id.erase(estado2);
    this->id_para_q.erase(id_estado2);
    // Remove o estado do vetor de estados finais
    this->finais.erase(std::remove(this->finais.begin(), this->finais.end(), id_estado2), this->finais.end());
    // Muda transições para o estado 2 para o estado 1
    for(unsigned int i = 0; i < this->transicoes.size(); i++){
        if(i != (unsigned int) id_estado2){
            for(auto &transicao: this->transicoes[i]){
                if(transicao.second == id_estado2){
                    this->transicoes[i][transicao.first] = id_estado1;
                }
            }
        }
    }
    // Remove o estado do vetor de transições
    this->transicoes.erase(id_estado2);
    // Muda o estado inicial se for o estado 2
    if (this->estado_inicial == id_estado2) {
        this->estado_inicial = id_estado1;
    }
}

void Automato::removeEstado(int estado) {
    // Pega nome do estado
    std::string nome_estado = this->getNomeEstado(estado);
    // Remove estado
    this->removeEstado(nome_estado);
}

std::string Automato::getNomeEstado(int i) {
    return this->id_para_q[i];
}

void Automato::simplifica() {
    printf("Simplificando automato %s\n", this->nome.c_str());
    printf("Removendo estados inalcancaveis\n");
    this->removeEstadosInalcancaveis();
    printf("Removendo estados mortos\n");
    this->removeEstadosMortos();
    printf("Unindo estados equivalentes\n");
    this->uneEstadosEquivalentes();
    printf("Removendo simbolos inuteis\n");
    this->removeSimbolosInuteis();
}

void Automato::removeEstadosInalcancaveis() {
    std::vector<int> estados_alcancaveis;
    std::vector<int> novos_estados_alcancaveis;
    std::vector<int> estados_visitados;
    novos_estados_alcancaveis.push_back(this->estado_inicial);
    estados_visitados.push_back(this->estado_inicial);
    while(estados_alcancaveis.size() != novos_estados_alcancaveis.size()){
        estados_alcancaveis = novos_estados_alcancaveis;
        for(auto &estado: estados_alcancaveis){
            for(auto &transicao: this->transicoes[estado]){
                if(transicao.second != -1 &&
                    std::find(estados_visitados.begin(), estados_visitados.end(), transicao.second) ==
                    estados_visitados.end()){
                    novos_estados_alcancaveis.push_back(transicao.second);
                    estados_visitados.push_back(transicao.second);
                }
            }
        }
    }
    std::vector<std::string> remover_estados;
    for(auto &estado: this->q_para_id){
        if(std::find(estados_alcancaveis.begin(), estados_alcancaveis.end(), estado.second) == estados_alcancaveis.end()){
            remover_estados.push_back(estado.first);
        }
    }
    for(auto &estado: remover_estados) {
        this->removeEstado(estado);
    }
}

void Automato::removeEstadosMortos() {
    std::vector<int> estados_mortos;
    for(auto &estado: this->q_para_id){
        if(std::find(this->finais.begin(), this->finais.end(), estado.second) == this->finais.end()){
            bool tem_transicao = false;
            for(auto &transicao: this->transicoes[estado.second]){
                if(transicao.second != estado.second){
                    tem_transicao = true;
                    break;
                }
            }
            if(!tem_transicao){
                estados_mortos.push_back(estado.second);
            }
        }
    }
    for(auto &estado: estados_mortos){
        this->removeEstado(estado);
    }
}

void Automato::uneEstadosEquivalentes() {
    // 1. Criar uma tabela de equivalência

    // Tabela de equivalência é uma matriz de booleanos,
    // onde a[i][j] é false se os estados i e j são equivalentes
    std::vector<std::vector<bool>> tabela_equivalencia;
    for(unsigned int i = 0; i < this->q_para_id.size(); i++){
        std::vector<bool> row(this->q_para_id.size(), false);
        tabela_equivalencia.push_back(row);
    }
    // 2. Marcar estados finais e não finais como não equivalentes
    for(auto &estado_final: this->finais){
        for(auto &estado: this->q_para_id){
            if(std::find(this->finais.begin(), this->finais.end(), estado.second) == this->finais.end()){
                tabela_equivalencia[estado_final][estado.second] = true;
                tabela_equivalencia[estado.second][estado_final] = true;
            }
        }
    }

    // 3. Marcar estados equivalentes
    bool foi_alterado = true;
    while(foi_alterado){
        foi_alterado = false;
        for(auto &estado1: this->q_para_id){
            for(auto &estado2: this->q_para_id){
                if(!tabela_equivalencia[estado1.second][estado2.second]){
                    if(estado1.second == estado2.second){
                        continue;
                    }
                    for(auto &simbolo: this->alfabeto){
                        int transicoes_estado1 = this->transicoes[estado1.second][simbolo];
                        int transicoes_estado2 = this->transicoes[estado2.second][simbolo];
                        if(transicoes_estado1 == -1 && transicoes_estado2 == -1){
                            continue;
                        }
                        if(transicoes_estado1 == -1 || transicoes_estado2 == -1){
                            tabela_equivalencia[estado1.second][estado2.second] = true;
                            tabela_equivalencia[estado2.second][estado1.second] = true;
                            foi_alterado = true;
                            break;
                        }
                        if(tabela_equivalencia[transicoes_estado1][transicoes_estado2]){
                            tabela_equivalencia[estado1.second][estado2.second] = true;
                            tabela_equivalencia[estado2.second][estado1.second] = true;
                            foi_alterado = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    // 4. Remover estados equivalentes
    for(unsigned int x = 0; x < tabela_equivalencia.size(); x++){
        for(unsigned int y = 0; y < tabela_equivalencia.size(); y++){
            if(!tabela_equivalencia[x][y] && x != y && x < y){
                this->uneEstados(this->getNomeEstado(x), this->getNomeEstado(y));
            }
        }
    }
}

void Automato::removeSimbolosInuteis(){
    std::vector<bool> ctrl(this->alfabeto.size(), false);
    for(unsigned int i = 0; i < this->alfabeto.size(); i++){
        for(auto &transicoes_estado: this->transicoes){
            for(auto &transicao: transicoes_estado.second){
                if((transicao.first == alfabeto[i]) && (transicao.second != -1)){
                    ctrl[i] = true;
                    break;
                }
            }
            if(ctrl[i] == true){
                break;
            }
        }
    }
    std::vector<char> novo_alfabeto;
    for(unsigned int i = 0; i < ctrl.size(); i++){
        if(ctrl[i] == true){
            novo_alfabeto.push_back(this->alfabeto[i]);
        }
        else{
            printf("Removendo '%c'\n", this->alfabeto[i]);
        }
    }
    this->alfabeto = novo_alfabeto;
}
