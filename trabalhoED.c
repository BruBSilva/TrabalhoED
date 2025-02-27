#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

typedef struct Pessoa {
    char nome[61];
    char sexo;
    double salario;
}Pessoa;

typedef struct Node {
    Pessoa* pessoa;
    struct Node* prev;
    struct Node* next;
}Node;

typedef struct Lista {
    Node* head;
    Node* tail;
}Lista;

Lista* createLista();
Pessoa* createPessoa(char* linha, int len);
Node* createNode(Pessoa* pessoa);
int isEmpty(Lista* lista);
void insert(Pessoa* pessoa, Lista* lista);
void deleteLista(Lista* lista);
void imprimeListaCresc(Lista* lista);
void imprimeListaDecresc(Lista* lista);

int main() {
    SetConsoleOutputCP(CP_UTF8);

    char linha[200];
    int len;

    FILE *arquivo = fopen("file example - ed", "r");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    Lista* lista = createLista();

    // Lendo linha por linha
    while (fgets(linha, 200, arquivo) != NULL) {
        len = strlen(linha);

        if (len == 0) continue;

        if (linha[len - 1] == '\n') {
            linha[len - 1] = '\0';
            len--;
        }

        Pessoa* pessoa = createPessoa(linha, len);

        insert(pessoa, lista);

    }

    int opcaoMenu;

    while(opcaoMenu != 3){
        printf("\n\nMenu:\n1 - mostrar dados em ordem crescente\n2 - mostrar dados em ordem decrescente\n3 - encerrar\n");
        scanf("%d", &opcaoMenu);

        switch (opcaoMenu) {
            case 1:
                imprimeListaCresc(lista);
                break;
            case 2:
                imprimeListaDecresc(lista);
                break;
            case 3:
                deleteLista(lista);
                break;
            default:
                printf("Opcao invalida\n");
                break;
        }
    }

    fclose(arquivo);

    return 0;
}

Lista* createLista(){
    Lista* lista = (Lista*) malloc(sizeof(Lista));

    if(lista == NULL){
        printf("Não foi possível criar a lista");
        exit(1);
    }

    lista->head = NULL;
    lista->tail = NULL;
    return lista;
}

Pessoa* createPessoa(char* linha, int len) {
    Pessoa* novaPessoa = (Pessoa*) malloc(sizeof(Pessoa));

    if (novaPessoa == NULL) {
        printf("Não foi possível criar nova pessoa");
        exit(1);
    }

    sscanf(linha, "%60c", &novaPessoa->nome);
    sscanf(linha + len - 9, "%lf", &novaPessoa->salario);
    novaPessoa->sexo = linha[len-10];

    //int i = strlen(novaPessoa->nome);
    int i = 60;
    while(i > 0 && isspace(linha[i])){
        i--;
    }
    novaPessoa->nome[i+1] = '\0';

    return novaPessoa;
}

Node* createNode(Pessoa* pessoa){
    Node* node = (Node*) malloc(sizeof(Node));

    if(node == NULL){
        printf("Não foi possível criar o nó");
        exit(1);
    }

    node->pessoa = pessoa;
    node->next = NULL;
    node->prev = NULL;
    return node;

}

int isEmpty(Lista* lista){
    return lista->head == NULL;
}

void insert(Pessoa* pessoa, Lista* lista){
    Node* newNode = createNode(pessoa);

    if(isEmpty(lista)){
        lista->head = newNode;
        lista->tail = newNode;
        return;
    }

    Node* currentNode = lista->head;
    Node* previousNode = NULL;

    while (currentNode != NULL && strcmp(currentNode->pessoa->nome, newNode->pessoa->nome) < 0) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (previousNode == NULL) {
        newNode->next = currentNode;
        currentNode->prev = newNode;
        lista->head = newNode;
    } else if (currentNode == NULL) {
        previousNode->next = newNode;
        newNode->prev = previousNode;
        lista->tail = newNode;
    } else {
        previousNode->next = newNode;
        newNode->prev = previousNode;
        newNode->next = currentNode;
        currentNode->prev = newNode;
    }
}

void deleteLista(Lista* lista){
    Node* currentNode = lista->head;
    Node* nextNode;

    while (currentNode != NULL) {
        nextNode = currentNode->next;

        free(currentNode->pessoa);
        free(currentNode);

        currentNode = nextNode;
    }

    free(lista);
}

void imprimeListaCresc(Lista* lista){
    Node* currentNode = lista->head;

    while (currentNode != NULL) {
        printf("------------------------------------------------------------\n");
        printf("Nome: %s\n", currentNode->pessoa->nome);
        printf("Sexo: %c\n", currentNode->pessoa->sexo);
        printf("Salário: %.2lf\n", currentNode->pessoa->salario);

        currentNode = currentNode->next;
    }
}

void imprimeListaDecresc(Lista* lista){
    Node* currentNode = lista->tail;

    while (currentNode != NULL) {
        printf("------------------------------------------------------------\n");
        printf("Nome: %s\n", currentNode->pessoa->nome);
        printf("Sexo: %c\n", currentNode->pessoa->sexo);
        printf("Salário: %.2lf\n", currentNode->pessoa->salario);

        currentNode = currentNode->prev;
    }
}
