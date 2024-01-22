#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#define VERDADE 1
#define FALSO 0
#define NAO_ENCONTRADO -1

#define ERRO_PARSING 1

/* Numero maximo de bytes que uma linha de comandos pode ter */
#define BUFSIZE 65535

typedef struct Paragem {
    char* id;
    double latitude;
    double longitude;
    int num_carreiras;
    struct Carreira* head;
    struct Paragem* prox;
} Paragem;

/* Estrutura Carreira */
typedef struct Carreira {
    char* id;
    struct Ligacao* head,* tail;
    double custo;
    double duracaoTotal;
    int num_ligacoes;
    struct Carreira* prox;
} Carreira;

/* Estrutura Ligacao */
typedef struct Ligacao {
    char* origem,* destino;
    double custo;
    double duracao;
    struct Ligacao* prox,* prev;
} Ligacao;

/* Função para dar free a TUDO */
void apagaMemoria(Carreira** head_carreira,Paragem** head_paragem);

/* FUNÇÃO PARA VERIFICAR SE FOI POSSíVEL ALOCAR MEMÒRIA */
void is_null(void *ptr,Carreira** head_carreira,Paragem** head_paragem);

/* FUNÇÕES PARA PARSING DO INPUT: */
int leEspacos();
char* leNome(Carreira** head_c,Paragem** head_p);
void leAteFinalLinha();

/* FUNÇÕES PARA TRATAR AS CARREIRAS */
Carreira* encontraCarreira(Carreira** head_carreira,char* id);
void mostraCarreira(Carreira* carreira);
void mostraLigacoesCarreira(Carreira** head,char* nome, int inverso);
void listaCarreiras(Carreira** head_carreira);
Carreira* NEWC(char* nome,Carreira** head_c,Paragem** head_p);
void criaCarreira(Carreira** head_c,char* nome,Paragem** head_p);
int verificaInversoOk(Carreira** head_c,Paragem** head_p);
void carreiras(Carreira** head_carreira,Paragem** head_paragem);

/* FUNÇÕES PARA TRATAR AS PARAGENS */
Paragem* encontraParagem(Paragem** head_paragem,char* id);
void mostraParagem(Paragem* paragem);
void listaParagens(Paragem** head_paragem);
Paragem* NEWP(char* nome,Carreira** head_c,Paragem** head_p);
void criaParagem(Paragem** head_p,char* nome, double latitude, double longitude,Carreira** head_c);
void paragens(Paragem** head_paragem,Carreira** head_carreira);

/* FUNÇÕES PARA TRATAR AS LIGAÇÕES */
Ligacao* encontraParagemNaCarreira(Carreira* carreira, char* id_paragem);
void leNomesComando(char** nomeCarreira,char** nomeOrigem,char** nomeDestino,Carreira** head_c,Paragem** head_p);
Ligacao* criaLigacao(char* origem,char* destino,double custo, double duracao,Carreira** head_c,Paragem** head_p);
void adicionaCarreiraParagem(Paragem** paragem,Carreira* carreira,Carreira** head_c,Paragem** head_p);
void acrescentaLigacaoInicio(Carreira** carreira,Ligacao* ligacao);
void acrescentaLigacaoFim(Carreira** carreira,Ligacao* ligacao);
void addPrimeiraLigacao(Carreira* carreira,Paragem* origem,Paragem* destino,double custo, double duracao,Carreira** head_c,Paragem** head_p);
void adicionaLigacao(Carreira* carreira,Paragem* origem,Paragem* destino,double custo, double duracao,Carreira** head_c,Paragem** head_p);
void ligacoes(Carreira** head_carreira,Paragem** head_paragem);

/* FUNÇÕES PARA TRATAR AS INTERSEÇÕES */
void ordenaCarreiras(Paragem *paragem, int numCarreiras);
void intersecoes(Paragem** head_paragem);

/* FUNÇÕES PARA TRATAR A REMOÇÃO DE UMA CARREIRA E SUAS LIGAÇÕES */
void freeCarreira(Carreira* carreira, Carreira** head_carreira);
void freeCarreiraParagens(Carreira* carreira, Paragem** head_paragem);
void freeLigacoesCarreira(Carreira* carreira);
void removecarreira(Carreira** head_carreira,Paragem** head_paragem);

/* FUNÇÕES PARA TRATAR A REMOÇÂO DE UMA PARAGEM DO SISTEMA */
void freeligacao(Ligacao* ligacao);
void freeCarreiraParagem(Carreira* carreira, Paragem* paragem,Paragem** head_paragem);
void caso_origem(Ligacao* current,Carreira** carreira,Paragem** head_paragem);
void caso_ultimo_destino(Ligacao* current,Carreira** carreira,Paragem** head_paragem);
void caso_destino(Ligacao* current,Carreira** carreira,Carreira** head_c,Paragem** head_p);
void editListaLigacoes(Paragem* paragem, Carreira** carreira,Carreira** head_carreira,Paragem** head_paragem);
void freeParagemLigacoesCarreira(Paragem* paragem,Carreira** head_carreira,Paragem** head_paragem);
void freeParagemListaCarreiras(Paragem* paragem);
void freeParagem(Paragem* paragem, Paragem** head_paragem);
void removeparagem(Carreira** head_carreira,Paragem** head_paragem);

#endif
