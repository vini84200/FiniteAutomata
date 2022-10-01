#include "automata.hpp"
#include <stdexcept>
#include <algorithm>
#include <iostream>


bool Automato::isFinal(const std::string& a) {
    int id = this->getStateId(a);
    return std::any_of(this->finals.begin(), this->finals.end(), [id](int s) { return s == id; });
}

bool Automato::isFinalInt(int a) {
    return std::any_of(this->finals.begin(), this->finals.end(), [a](int s) { return s == a; });
}

int Automato::getStateId(const std::string &s) {
    try {
        return this->q_to_id.at(s);
    } catch (std::out_of_range &ex) {
        return -1;
    }
}

int Automato::nextState(int current_s, char read) {
    try {
        return this->transitions.at(current_s).at(read);
    } catch (std::out_of_range &ex) {
        return -1;
    }
}

bool Automato::isAccepted(const std::string& word) {
    int q = this->initial_state;

    for (char c: word) {
        if (q == -1) {
            return isFinalInt(q);
        }
        q = nextState(q, c);
    }
    return isFinalInt(q);
}

void Automato::createState(const std::string& state_name) {
    if (this->getStateId(state_name) != -1) {
        return;
    }
    int id = ++this->last_id;
    this->q_to_id[state_name] = id;
    this->id_to_q[id] = state_name;
    this->transitions[id] = std::map<char, int>();
    for (char c: this->alphabet) {
        this->transitions[id][c] = -1;
    }
}

void Automato::createTransition(const std::string &q1, const std::string &q2, char c) {
    int s1 = this->getStateId(q1);
    int s2 = this->getStateId(q2);
    if (s1 == -1 || s2 == -1) {
        return;
    }

    this->transitions[s1][c] = s2;
}

Automato::Automato(const std::string &name, const std::vector<char> &alphabet,
                   const std::vector<std::string> &states, const std::vector<std::string> &finals,
                   const std::string &initial_state) {

    this->name = name;
    for (char i: alphabet) {
        this->alphabet.push_back(i);
    }

    last_id = -1;
    createState(initial_state);
    this->initial_state = getStateId(initial_state);

    for (auto &final: finals) {
        createState(final);
        this->finals.push_back(getStateId(final));
    }

    for (auto &state: states) {
        createState(state);
    }


}

void Automato::print() {
    std::cout << "Automato " << name << std::endl;
    std::cout << "Alfabeto: ";
    for (auto &letter: alphabet) {
        std::cout << letter << " ";
    }
    std::cout << std::endl;
    std::cout << "Estados: ";
    for (auto &state: id_to_q) {
        std::cout << state.second << " ";
    }
    std::cout << std::endl;
    std::cout << "Estado inicial: " << getStateName(initial_state) << std::endl;
    std::cout << "Estados finais: ";
    for (auto &final: finals) {
        std::cout << getStateName(final) << " ";
    }
    std::cout << std::endl;
    std::cout << "Transições: " << std::endl;
    for (int i = 0; i < transitions.size(); i++) {
        for (auto &transition: transitions[i]) {
            if (transition.second != -1) {
                std::cout << getStateName(i) << " -> " << getStateName(transition.second) << " : " << transition.first
                          << std::endl;
            }
        }
    }
    std::cout << std::endl;

}

void Automato::removeState(const std::string &state) {
    printf("removing state %s\n", state.c_str());
    int state_id = getStateId(state);
    if (state_id == -1) {
        return;
    }
    // remove from id_to_q
    id_to_q.erase(state_id);
    // remove from q_to_id
    q_to_id.erase(state);

    // remove from finals
    this->finals.erase(std::remove(this->finals.begin(), this->finals.end(), this->getStateId(state)),
                       this->finals.end());
    // remove from transitions
    for (int i = 0; i < this->transitions.size(); i++) {
        if (i != state_id) {
            std::vector<char> to_remove;
            for (auto &transition: this->transitions[i]) {
                if (transition.second == state_id) {
                    to_remove.push_back(transition.first);
                }
            }
            for (auto &c: to_remove) {
                this->transitions[i].erase(c);
            }
        }
    }
    this->transitions.erase(state_id);
}

void Automato::merge_states(const std::string &state1, const std::string &state2) {
    printf("merging states %s and %s\n", state1.c_str(), state2.c_str());
    int state1_id = getStateId(state1);
    int state2_id = getStateId(state2);
    // Remove o estado do vetor de estados
    this->q_to_id.erase(state2);
    this->id_to_q.erase(state2_id);
    // Remove o estado do vetor de estados finais
    this->finals.erase(std::remove(this->finals.begin(), this->finals.end(), state2_id), this->finals.end());
    // Muda transições para o estado 2 para o estado 1
    for (int i = 0; i < this->transitions.size(); i++) {
        if (i != state2_id) {
            for (auto &transition: this->transitions[i]) {
                if (transition.second == state2_id) {
                    this->transitions[i][transition.first] = state1_id;
                }
            }
        }
    }
    // Remove o estado do vetor de transições
    this->transitions.erase(state2_id);
    // Muda o estado inicial se for o estado 2
    if (this->initial_state == state2_id) {
        this->initial_state = state1_id;
    }
}

void Automato::removeState(int state) {
    // Get state name
    std::string state_name = this->getStateName(state);
    // Remove state
    this->removeState(state_name);
}

std::string Automato::getStateName(int i) {
    return this->id_to_q[i];
}

void Automato::simplify() {
    printf("Simplifying automato %s\n", this->name.c_str());
    printf("Removing unreachable states\n");
    this->removeUnreachableStates();
    printf("Removing dead states\n");
    this->removeDeadStates();
    printf("Merging equivalent states\n");
    this->mergeEquivalentStates();

}

void Automato::removeUnreachableStates() {
    std::vector<int> reachable_states;
    std::vector<int> new_reachable_states;
    std::vector<int> visited_states;
    new_reachable_states.push_back(this->initial_state);
    visited_states.push_back(this->initial_state);
    while (reachable_states.size() != new_reachable_states.size()) {
        reachable_states = new_reachable_states;
        for (auto &state: reachable_states) {
            for (auto &transition: this->transitions[state]) {
                if (transition.second != -1 &&
                    std::find(visited_states.begin(), visited_states.end(), transition.second) ==
                    visited_states.end()) {
                    new_reachable_states.push_back(transition.second);
                    visited_states.push_back(transition.second);
                }
            }
        }
    }
    std::vector<std::string> states_to_remove;
    for (auto &state: this->q_to_id) {
        if (std::find(reachable_states.begin(), reachable_states.end(), state.second) == reachable_states.end()) {
            states_to_remove.push_back(state.first);
        }
    }
    for (auto &state: states_to_remove) {
        this->removeState(state);
    }
}

void Automato::removeDeadStates() {
    std::vector<int> dead_states;
    for (auto &state: this->q_to_id) {
        if (std::find(this->finals.begin(), this->finals.end(), state.second) == this->finals.end()) {
            bool has_transition = false;
            for (auto &transition: this->transitions[state.second]) {
                if (transition.second != state.second) {
                    has_transition = true;
                    break;
                }
            }
            if (!has_transition) {
                dead_states.push_back(state.second);
            }
        }
    }
    for (auto &state: dead_states) {
        this->removeState(state);
    }
}

void Automato::mergeEquivalentStates() {
    // 1. Criar uma tabela de equivalência

    // Tabela de equivalência é uma matriz de booleanos,
    // onde a[i][j] é false se os estados i e j são equivalentes
    std::vector<std::vector<bool>> equivalence_table;
    for (int i = 0; i < this->q_to_id.size(); i++) {
        std::vector<bool> row(this->q_to_id.size(), false);
        equivalence_table.push_back(row);
    }

    // 2. Marcar estados finais e não finais como não equivalentes
    for (auto &final_state: this->finals) {
        for (auto &state: this->q_to_id) {
            if (std::find(this->finals.begin(), this->finals.end(), state.second) == this->finals.end()) {
                equivalence_table[final_state][state.second] = true;
                equivalence_table[state.second][final_state] = true;
            }
        }
    }

    // 3. Marcar estados equivalentes
    bool has_changed = true;
    while (has_changed) {
        has_changed = false;
        for (auto &state1: this->q_to_id) {
            for (auto &state2: this->q_to_id) {
                if (!equivalence_table[state1.second][state2.second]) {
                    if (state1.second == state2.second) continue;
                    for (auto &symbol: this->alphabet) {
                        int state1_transition = this->transitions[state1.second][symbol];
                        int state2_transition = this->transitions[state2.second][symbol];
                        if (state1_transition == -1 && state2_transition == -1) continue;
                        if (state1_transition == -1 || state2_transition == -1) {
                            equivalence_table[state1.second][state2.second] = true;
                            equivalence_table[state2.second][state1.second] = true;
                            has_changed = true;
                            break;
                        }
                        if (equivalence_table[state1_transition][state2_transition]) {
                            equivalence_table[state1.second][state2.second] = true;
                            equivalence_table[state2.second][state1.second] = true;
                            has_changed = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    // 4. Remover estados equivalentes
    for (int x = 0; x < equivalence_table.size(); x++) {
        for (int y = 0; y < equivalence_table.size(); y++) {
            if (!equivalence_table[x][y] && x != y && x < y) {
                this->merge_states(this->getStateName(x), this->getStateName(y));
            }
        }
    }
}
