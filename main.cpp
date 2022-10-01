#include "automato.hpp"
#include "parser.h"
#include <cstring>

int main(){
    printf("Qual arquivo deseja abrir? ");
    char nome_arq[256];
    scanf("%s", nome_arq);
    if (strlen(nome_arq) == 0){
        printf("Nome de arquivo invalido");
        return 1;
    }
    Parser parser;
    Automato automato = parser.parseArquivo(nome_arq);

    bool ctrl = false;
    while(!ctrl){
        printf("O que deseja fazer?\n");
        printf("1 - Simplificar o automato\n");
        printf("2 - Testar uma palavra\n");
        printf("3 - Testar uma lista de palavras com um arquivo\n");
        printf("4 - Imprimir o automato\n");
        printf("5 - Sair\n");
        int opcao;
        if(scanf("%d", &opcao) == 0){
            printf("Opcao invalida\n");
            break;
        }
        switch(opcao){
            case 1:
                automato.simplifica();
                break;
            case 2:{
                printf("Digite a palavra: ");
                char palavra[256];
                scanf("%s", palavra);
                if (automato.ehAceita(palavra)){
                    printf("A palavra %s e aceita pelo automato\n", palavra);
                }else{
                    printf("A palavra %s nao e aceita pelo automato\n", palavra);
                }
                break;
            }
            case 3:{
                printf("Digite o nome do arquivo: ");
                nome_arq[0] = '\0';
                scanf("%s", nome_arq);
                if (strlen(nome_arq) == 0) {
                    printf("Nome de arquivo invalido");
                    break;
                }
                FILE *file = fopen(nome_arq, "r");
                if (file == nullptr){
                    printf("Arquivo nao encontrado");
                    break;
                }
                while (!feof(file)){
                    char palavra[256];
                    fgets(palavra, 100, file);
                    palavra[strlen(palavra) - 1] = '\0';
                    if(automato.ehAceita(palavra)){
                        printf("A palavra '%s' e aceita pelo automato\n", palavra);
                    }else{
                        printf("A palavra '%s' nao e aceita pelo automato\n", palavra);
                    }
                }
                fclose(file);
                break;
            }
            case 4:
                automato.imprime();
                break;
            case 5:
                ctrl = true;
                printf("Obrigado por usar o programa de simplificacao de automatos!\n");
                printf("Desenvolvido por Arthur Alves Ferreira Melo - 00333985, Joao Pedro Kuhn Braun - 00325265"
                       " e Vinicius Jose Fritzen - 00335519\n");
                break;
        }
    }
    return 0;
}

