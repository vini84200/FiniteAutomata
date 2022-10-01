#include <map>
#include <vector>
#include <string>

#ifndef AUTOMATA_HPP
#define AUTOMATA_HPP

class Automato{
private:
    std::string nome;
    std::vector<char> alfabeto;
    std::map<std::string, int> q_para_id;
    std::map<int, std::string> id_para_q;
    std::map<int, std::map<char, int>> transicoes;
    std::vector<int> finais;
    int estado_inicial;
    int ultimo_id;

    /// Retorna id de estado
    int getIdEstado(const std::string &s);
    /// Retorna id do próximo estado
    int proxEstado(int string_atual, char lido);
    /// Testa se id é final
    bool ehIntFinal(int q);
    /// Remove estado por nome de estado
    void removeEstado(const std::string &estado);
    /// Remove estado por id
    void removeEstado(int estado);
    /// Une estados a partir de nomes de estado
    void uneEstados(const std::string &estado1, const std::string &estado2);
    /// Remove estados impossíveis de serem alcançados
    void removeEstadosInalcancaveis();
    /// Remove estados mortos
    void removeEstadosMortos();
    /// Une todos estados equivalentes
    void uneEstadosEquivalentes();
    /// Remove símbolos que não foram utilizados
    void removeSimbolosInuteis();

public:

    /// Constrói um novo Automato
    ///@param _nome Nome do automato
    ///@param _alfabeto Vetor de caracteres que pertencem ao alfabeto
    ///@param estados Vetor com o nome de todos os estados
    ///@param _finais Vetor com os estados finais
    ///@param _estado_inicial Nome do estado inicial
    Automato(const std::string &_nome,
             const std::vector<char> &_alfabeto,
             const std::vector<std::string> &estados,
             const std::vector<std::string> &_finais,
             const std::string &_estado_inicial);

    /// Testa se estado é final
    /// @param a Nome do estado
    /// @return Se o estado é final
    bool ehFinal(const std::string& a);

    /// Testa se palavra é aceita
    /// \param palavra: String Palavra do alfabeto para verificar
    /// @return Se a palavra é aceita.
    bool ehAceita(const std::string& palavra);

    /// Cria estado a partir de nome de estado
    /// \param nome_estado Nome do novo estado
    void criaEstado(const std::string& nome_estado);

    /// Cria transição a partir de nomes de estados e caractere
    /// \param q1 Estado de onde a transição começa
    /// \param c Destino da transição
    /// \param q2 Estado de destino da program
    void criaTransicao(const std::string &q1, const std::string &q2, char c);

    /// Imprime autômato
    void imprime();

    /// Simplifica autômato
    void simplifica();

    /// Retorna nome de estado
    std::string getNomeEstado(int i);
};

#endif
