#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <wchar.h>

typedef struct acao {
    wchar_t texto[100];
} Acao;

typedef struct no {
    Acao acao;
    struct no *proximo;
} No;

typedef struct {
    No *topo;
    int tam;
} Pilha;

// Função para inicializar a pilha
void iniciar_pilha(Pilha *p) {
    p->topo = NULL;
    p->tam = 0;
}

// Função para verificar se a pilha está vazia
int vazia(Pilha *p) {
    return p->topo == NULL;
}

// Operação push
void empilhar(Pilha *p, Acao acao) {
    No *no = (No *)malloc(sizeof(No));
    if (no) { 
        no->acao = acao;
        no->proximo = p->topo;
        p->topo = no;
        p->tam++;
    } else {
        wprintf(L"Erro ao usar memória para a pilha.\n");
    }
}

// Operação pop (desempilhar)
No* desempilhar(Pilha *p) {
    No *no = NULL;
    if (p->topo != NULL) {
        no = p->topo;
        p->topo = no->proximo;
        p->tam--;
    }
    return no;
}

// Função para realizar uma nova ação
void realizar_acao(Pilha *acoes, Pilha *refazer, const wchar_t *texto_acao) { // const wchar_t*
    Acao acao_nova;
    wcscpy(acao_nova.texto, texto_acao);
    empilhar(acoes, acao_nova);
    // Limpa a pilha de refazer ao realizar uma nova ação
    while (!vazia(refazer)) {
        No *acao_refazer = desempilhar(refazer);
        free(acao_refazer);
    }
}

// Função para desfazer uma ação
void desfazer(Pilha *acoes, Pilha *refazer) {
    if (!vazia(acoes)) {
        No *acao_desfeita = desempilhar(acoes);
        empilhar(refazer, acao_desfeita->acao);
        free(acao_desfeita);
    } else {
        wprintf(L"Não há ações disponíveis para desfazer.\n");
    }
}

// Função para refazer uma ação
void refazer_acao(Pilha *acoes, Pilha *refazer) {
    if (!vazia(refazer)) {
        No *acao_refazer = desempilhar(refazer);
        empilhar(acoes, acao_refazer->acao);
        free(acao_refazer);
    } else {
        wprintf(L"Não há ações disponíveis para refazer.\n");
    }
}

// Função para exibir as ações realizadas
void mostrar_acoes(Pilha *p) {
    No *recente = p->topo;
    wprintf(L"Ações realizadas:\n");
    while (recente != NULL) {
        wprintf(L" - %ls\n", recente->acao.texto);
        recente = recente->proximo;
    }
}

// Função principal para simular o editor de texto
int main() {
    _setmode(_fileno(stdout), _O_U8TEXT);
    Pilha acoes, refazer;
    iniciar_pilha(&acoes);
    iniciar_pilha(&refazer);

    // Simulando ações do usuário
    realizar_acao(&acoes, &refazer, L"Escreveu: Olá");
    realizar_acao(&acoes, &refazer, L"Escreveu: Mundo");
    desfazer(&acoes, &refazer); // Desfaz "Escreveu: Mundo"
    realizar_acao(&acoes, &refazer, L"Escreveu: Pessoal");

    // Exibe o estado final das ações
    mostrar_acoes(&acoes);

    return 0;
}


