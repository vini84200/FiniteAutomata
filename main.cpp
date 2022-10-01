#include "automata.hpp"
#include "parser.h"
#include <string.h>


int main() {
    printf("Qual arquivo deseja abrir? ");
    char file_name[256];
    scanf("%s", file_name);
    if (strlen(file_name) == 0) {
        printf("Nome de arquivo inválido");
        return 1;
    }
    Parser parser;
    Automato automato = parser.parseFile(file_name);
    bool quit = false;
    while (!quit) {
        printf("O que deseja fazer?\n");
        printf("1 - Simplificar o autômato\n");
        printf("2 - Testar uma palavra\n");
        printf("3 - Testar uma lista de palavras com um arquivo\n");
        printf("4 - Imprimir o autômato\n");
        printf("5 - Sair\n");
        int option;
        if (scanf("%d", &option) == 0) {
            printf("Opção inválida\n");
            break;
        }


        switch (option) {
            case 1:
                automato.simplify();
                break;

            case 2: {
                printf("Digite a palavra: ");
                char word[256];
                scanf("%s", word);
                if (automato.isAccepted(word)) {
                    printf("A palavra %s é aceita pelo autômato\n", word);
                } else {
                    printf("A palavra %s não é aceita pelo autômato\n", word);
                }
                break;
            }

            case 3: {
                printf("Digite o nome do arquivo: ");
                file_name[0] = '\0';
                scanf("%s", file_name);
                if (strlen(file_name) == 0) {
                    printf("Nome de arquivo inválido");
                    break;
                }
                FILE *file = fopen(file_name, "r");
                if (file == nullptr) {
                    printf("Arquivo não encontrado");
                    break;
                }
                while (!feof(file)) {
                    char word[256];
                    fgets(word, 100, file);
                    word[strlen(word) - 1] = '\0';
                    if (automato.isAccepted(word)) {
                        printf("A palavra '%s' é aceita pelo autômato\n", word);
                    } else {
                        printf("A palavra '%s' não é aceita pelo autômato\n", word);
                    }
                }
                fclose(file);
                break;
            }
            case 4:
                automato.print();
                break;
            case 5:
                quit = true;
                printf("Obrigado por usar o programa de simplificação de autômatos!\n");
                printf("Desenvolvido por Arthur Alves Ferreira Melo - 00333985, João Pedro Kuhn Braun - 00325265"
                       " e Vinicius José Fritzen - 00335519\n");
                break;
        }
    }

}
