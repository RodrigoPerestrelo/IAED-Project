/* iaed-23 - ist1106074 - project2 */

/*
 * Ficheiro: project2.c
 * Autor: Rodrigo Salvador dos Santos Perestrelo, ist1106074
 * Descricao: Este programa visa dar uma solucao ao problema/exercicio exposto
no enunciado(projeto2).
 * Nota: O numero de frees que faltam em relacao aos allocs, encontram-se no
comando 'e' (remoção de paragens).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Prototipos.h"

/* FUNÇÃO PARA VERIFICAR SE FOI POSSíVEL ALOCAR MEMÒRIA */
void is_null(void *ptr,Carreira** head_carreira,Paragem** head_paragem) {
    if (ptr == NULL) {
        printf("No memory.\n");
        apagaMemoria(head_carreira,head_paragem);
        exit(1);
    }
} 


/* FUNÇÕES PARA PARSING DO INPUT: */

/* Le espaços. Devolve 0 se chegou ao final de linha ou 1 caso contrário. */
int leEspacos() {
    int c;

    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return 0;
    ungetc(c,stdin);
    return 1;
}

/* Le um nome alocando memória dinamicamente. */
char* leNome(Carreira** head_c,Paragem** head_p) {
    char s[BUFSIZE];
    char* nome;
    int i=0,c,len;

    s[0] = getchar();
    if (s[0] != '"') {
        i = 1;
        while ((c = getchar()) != ' ' && c != '\t' && c != '\n')
            s[i++] = c;
        ungetc(c, stdin);
    }
    else {
        while((c = getchar()) != '"')
            s[i++] = c;
    }
    s[i++] = '\0';

    len = strlen(s);
    nome = (char*) malloc(sizeof(char)* (len+1));
    is_null(nome,head_c,head_p);
    strcpy(nome,s);

    return nome;
}

/* Le todo o texto até ao final de linha. */
void leAteFinalLinha() {
    char c;
    while ((c = getchar()) != '\n');
}


/* FUNÇÕES PARA TRATAR AS CARREIRAS */

/* Procura uma carreira com um determinado nome.
   Devolve o ponteiro da carreira ou NULL se não existe. */
Carreira* encontraCarreira(Carreira** head_carreira,char* id) {
    Carreira* carreira;

    for (carreira=(*head_carreira);carreira!= NULL;carreira=carreira->prox)
        if (!strcmp(carreira->id,id))
            return carreira;
    return NULL;
}

/* Escreve a carreira cujo ponteiro foi recebido como argumento */
void mostraCarreira(Carreira* carreira) {
    int numLigacoes = carreira->num_ligacoes;

    printf("%s ",carreira->id);
    if (numLigacoes > 0) {
        printf("%s %s ", carreira->head->origem,carreira->tail->destino);
        printf("%d ", numLigacoes+1); /*numero de paragens */
        printf("%.2f ", carreira->custo);
        printf("%.2f\n", carreira->duracaoTotal);
    }
    else 
        printf("%d %.2f %.2f\n", 0, 0.0, 0.0);
}

/* Escreve as ligações de uma carreira cujo nome foi recebido como argumento.
   Por ordem normal ou inversa */
void mostraLigacoesCarreira(Carreira** head,char* nome, int inverso) {
    int numLigacoes;
    Carreira* x;
    Ligacao* ligacao;

    x = encontraCarreira(head,nome);
    numLigacoes = x->num_ligacoes;

    if (numLigacoes == 0) /* se nao ha ligacoes a escrever */
        return;
    if (inverso == FALSO) { /* pela ordem origem-destino */
        ligacao = x->head;
        printf("%s, ",ligacao->origem);

        while (ligacao->prox != NULL) {
        printf("%s, ",ligacao->destino);
        ligacao = ligacao->prox;
        }
        printf("%s\n", ligacao->destino);
    }

    else { /* pela ordem destino-origem */
        ligacao = x->tail;
        printf("%s, ",ligacao->destino);

        while (ligacao->prev != NULL) {
        printf("%s, ",ligacao->origem);
        ligacao = ligacao->prev;
        }
        printf("%s\n",ligacao->origem);
    }
}

/* Escreve todas as carreiras da lista ligada de carreiras. */
void listaCarreiras(Carreira** head_carreira) {
    Carreira* carreira = (*head_carreira);

    while(carreira != NULL) {
        mostraCarreira(carreira);
        carreira = carreira->prox;
    }
}

/* Cria o espaço de memória de uma carreira nova e preenche o seu nome. */
Carreira* NEWC(char* nome,Carreira** head_c,Paragem** head_p) {
    Carreira* x = (Carreira*) malloc(sizeof(struct Carreira));
    is_null(x,head_c,head_p);

    x->id = (char*) malloc(sizeof(char)*(strlen(nome)+1));
    strcpy(x->id, nome);
    x->prox = NULL;
    return x;
}

/* Cria uma carreira nova na lista de carreiras, inicializando-a. */
void criaCarreira(Carreira** head_c,char* nome,Paragem** head_p) {
    Carreira* x;

    if ((*head_c) == NULL) {
        (*head_c) = NEWC(nome,head_c,head_p);
        (*head_c)->custo = 0.0;
        (*head_c)->duracaoTotal = 0.0;
        (*head_c)->num_ligacoes = 0;
        (*head_c)->head = NULL;
        (*head_c)->tail = NULL;
        return;
    }
    for (x = *head_c; x->prox != NULL; x = x->prox);
    x->prox = NEWC(nome,head_c,head_p);
    x->prox->custo = 0.0;
    x->prox->duracaoTotal = 0.0;
    x->prox->num_ligacoes = 0;
    x->prox->head = NULL;
    x->prox->tail = NULL;
}

/* Verifica se a string é um prefixo de tamanho pelo menos 3 da
   palavra inverso. */
int verificaInversoOk(Carreira** head_c,Paragem** head_p) {
    char inv[] = "inverso";
    int size, i;
    char* s;
    s = leNome(head_c,head_p);
    size = strlen(s);

    if (size < 3 || size > 7) {
        free(s);
        return FALSO;
    }
    for (i = 0; i < size; i++) {
        if (inv[i] != s[i]) {
            free(s);
            return FALSO;
        }
    }
    free(s);
    return VERDADE;
}

/* Função para tratar do comando 'c'. */
void carreiras(Carreira** head_carreira,Paragem** head_paragem) {
    char* nome;
    Carreira* carreira;
    int fimLinha = leEspacos();
    
    if (!fimLinha) {
        listaCarreiras(head_carreira);
        return;
    }

    nome = leNome(head_carreira,head_paragem);
    carreira = encontraCarreira(head_carreira,nome);
    fimLinha = leEspacos();
    if (!fimLinha) {
        if (carreira == NULL)
            criaCarreira(head_carreira,nome,head_paragem);
        else
            mostraLigacoesCarreira(head_carreira,nome,FALSO);
    }
    else {
        if (verificaInversoOk(head_carreira,head_paragem))
            mostraLigacoesCarreira(head_carreira,nome,VERDADE);
        else
            printf("incorrect sort option.\n");
        leAteFinalLinha();
    }
    free(nome);
}


/* FUNÇÕES PARA TRATAR AS PARAGENS */

/* Procura uma paragem com um determinado nome.
   Se existir devolve o ponteiro. Caso contrário, devolve NULL. */
Paragem* encontraParagem(Paragem** head_paragem,char* id) {
    Paragem* paragem;

    for (paragem=(*head_paragem);paragem!=NULL;paragem=paragem->prox)
        if (!strcmp(paragem->id,id))
            return paragem;
    return NULL;
}

/* Escreve a paragem cujo ponteiro foi recebido como argumento */
void mostraParagem(Paragem* paragem) {
    printf("%s: %16.12f %16.12f %d\n", paragem->id, paragem->latitude, 
            paragem->longitude, paragem->num_carreiras);
}

/* Escreve todas as paragens da lista ligada de paragens. */
void listaParagens(Paragem** head_paragem) {
    Paragem* paragem = (*head_paragem);

    while(paragem != NULL) {
        mostraParagem(paragem);
        paragem = paragem->prox;
    }
}

/* Cria o espaço de memória de uma paragem nova e preenche o seu nome. */
Paragem* NEWP(char* nome,Carreira** head_c,Paragem** head_p) {
    Paragem* x = (Paragem*) malloc(sizeof(struct Paragem));

    x->id = (char*) malloc(sizeof(char)*(strlen(nome)+1));
    is_null(x->id,head_c,head_p);
    strcpy(x->id, nome);
    x->prox = NULL;
    return x;
}

/* Cria uma paragem nova na lista de paragens, inicializando-a. */
void criaParagem(Paragem** head_p,char* nome, double latitude, double longitude,
                 Carreira** head_c) {
    Paragem* x;

    if (*head_p == NULL) {
        *head_p = NEWP(nome,head_c,head_p);
        (*head_p)->latitude = latitude;
        (*head_p)->longitude = longitude;
        (*head_p)->num_carreiras = 0;
        (*head_p)->head = NULL;
        return;
    }
    for (x = *head_p; x->prox != NULL; x = x->prox);
    x->prox = NEWP(nome,head_c,head_p);
    x->prox->latitude = latitude;
    x->prox->longitude = longitude;
    x->prox->num_carreiras = 0;
    x->prox->head = NULL;
}

/* Função para tratar o comando 'p'. */
void paragens(Paragem** head_paragem,Carreira** head_carreira) {
    char* nome;
    Paragem* paragem;
    int fimLinha = leEspacos();
    
    if (!fimLinha) {
        listaParagens(head_paragem);
        return;
    }

    nome = leNome(head_carreira,head_paragem);
    paragem = encontraParagem(head_paragem,nome);
    fimLinha = leEspacos();
    if (!fimLinha) {
        if (paragem== NULL)
            printf("%s: no such stop.\n",nome);
        else   
            printf("%16.12f %16.12f\n",paragem->latitude,paragem->longitude);
    }
    else {
        double latitude, longitude;
        scanf("%lf%lf", &latitude, &longitude);
        if (paragem == NULL)
            criaParagem(head_paragem,nome,latitude,longitude,head_carreira);
        else
            printf("%s: stop already exists.\n",nome);
        leAteFinalLinha();
    }
    free(nome);
}


/* FUNÇÕES PARA TRATAR AS LIGAÇÕES */

/* Procura pelo nome se uma paragem existe na lista de ligacoes de uma carreira.
   Se existir, devolve a primeira ligação que usa a paragem. 
   Caso contrário, devolve NULL. */
Ligacao* encontraParagemNaCarreira(Carreira* carreira, char* id_paragem) {
    Ligacao* ligacao = carreira->head;

    while (ligacao->prox != NULL) {
        if (strcmp(ligacao->origem,id_paragem) == 0)
            return ligacao;
        ligacao = ligacao->prox;
    }
    if (!strcmp(ligacao->origem,id_paragem)||
        !strcmp(ligacao->destino,id_paragem))
        return ligacao;
    return NULL;
}

/* Le o nome da carreira e das paragens de uma ligacao. */
void leNomesComando(char** nomeCarreira,char** nomeOrigem,char** nomeDestino,
                    Carreira** head_c,Paragem** head_p) {
    leEspacos();
    (*nomeCarreira) = leNome(head_c,head_p);
    leEspacos();
    (*nomeOrigem) = leNome(head_c,head_p);
    leEspacos();
    (*nomeDestino) = leNome(head_c,head_p);
}

/* Cria uma ligacao nova e o espaço de memoria necessário, inicializando-a. */
Ligacao* criaLigacao(char* origem,char* destino,double custo, double duracao,
                     Carreira** head_c,Paragem** head_p) {
    Ligacao* ligacao = (Ligacao*) malloc(sizeof(struct Ligacao));
    is_null(ligacao,head_c,head_p);

    ligacao->origem = (char*) malloc(sizeof(char)*(strlen(origem)+1));
    is_null(ligacao->origem,head_c,head_p);
    strcpy(ligacao->origem, origem);

    ligacao->destino = (char*) malloc(sizeof(char)*(strlen(destino)+1));
    is_null(ligacao->destino,head_c,head_p);
    strcpy(ligacao->destino, destino);

    ligacao->custo = custo;
    ligacao->duracao = duracao;
    ligacao->prox = NULL;
    ligacao->prev = NULL;

    return ligacao;
}

/* Adiciona uma nova carreira à lista ligada de carreiras(ids) de uma paragem.
   Atualiza o numero de carreiras que passam na paragem(+1). */
void adicionaCarreiraParagem(Paragem** paragem,Carreira* carreira,
                             Carreira** head_c,Paragem** head_p) {
    Carreira* new = (Carreira*) malloc(sizeof(Carreira));
    new->id = (char*) malloc(sizeof(char)* (strlen(carreira->id)+1));
    is_null(new->id,head_c,head_p);

    (*paragem)->num_carreiras++;
    strcpy(new->id,carreira->id);
    
    new->prox = (*paragem)->head;
    (*paragem)->head = new;
}

/* Acrescenta uma nova ligação no início da lista de ligações de uma carreira.
   Atualiza o numero de ligacoes da carreira. */
void acrescentaLigacaoInicio(Carreira** carreira,Ligacao* ligacao) {
     if ((*carreira)->head == NULL) {
        (*carreira)->tail = ligacao;
    } else {
        ligacao->prox = (*carreira)->head;
        (*carreira)->head->prev = ligacao;

    }
    (*carreira)->head = ligacao;
    (*carreira)->custo += ligacao->custo;
    (*carreira)->duracaoTotal += ligacao->duracao;
    (*carreira)->num_ligacoes++;
}

/* Acrescenta uma nova ligação no fim da lista de ligações de uma carreira.
   Atualiza o numero de ligacoes da carreira. */
void acrescentaLigacaoFim(Carreira** carreira,Ligacao* ligacao) {
     if ((*carreira)->head == NULL) {
        (*carreira)->head = ligacao;
    } else {
        ligacao->prev = (*carreira)->tail;
        (*carreira)->tail->prox = ligacao;
    }
    
    (*carreira)->tail = ligacao;
    (*carreira)->custo += ligacao->custo;
    (*carreira)->duracaoTotal += ligacao->duracao;
    (*carreira)->num_ligacoes++;
}

/* Adiciona a primeira ligacao na lista de ligacoes de uma carreira. */
void addPrimeiraLigacao(Carreira* carreira,Paragem* origem,Paragem* destino,
    double custo, double duracao,Carreira** head_c,Paragem** head_p) {
    Ligacao* ligacao;

    adicionaCarreiraParagem(&origem,carreira,head_c,head_p);
    if (strcmp(origem->id,destino->id))
        adicionaCarreiraParagem(&destino,carreira,head_c,head_p);
    
    ligacao = criaLigacao(origem->id,destino->id,custo,duracao,head_c,head_p);
    acrescentaLigacaoFim(&carreira,ligacao);
}

/* Adiciona uma nova ligação à lista de ligações de uma carreira.
   verificando se é possível. */
void adicionaLigacao(Carreira* carreira,Paragem* origem,Paragem* destino,
    double custo, double duracao,Carreira** head_c,Paragem** head_p) {
    int numLigacoesCarreira = carreira->num_ligacoes;
    Ligacao* ligacao;
    
    if (numLigacoesCarreira == 0) 
        addPrimeiraLigacao(carreira,origem,destino,custo,duracao,head_c,head_p);
    else {
        if (!strcmp((origem->id),carreira->tail->destino)) {
            if (encontraParagemNaCarreira(carreira,destino->id) == NULL)
                adicionaCarreiraParagem(&destino,carreira,head_c,head_p);
            ligacao = criaLigacao(origem->id,destino->id,custo,duracao,
                                                        head_c,head_p);
            acrescentaLigacaoFim(&carreira,ligacao);
        }
        else if (!strcmp((destino->id),carreira->head->origem)) {
            if (encontraParagemNaCarreira(carreira,origem->id) == NULL)
                adicionaCarreiraParagem(&origem,carreira,head_c,head_p);
            ligacao = criaLigacao(origem->id,destino->id,custo,duracao,
                                                        head_c,head_p);
            acrescentaLigacaoInicio(&carreira,ligacao);
        }
        else
            printf("link cannot be associated with bus line.\n");
    }
}

/* Função para tratar o comando 'l'. */
void ligacoes(Carreira** head_carreira,Paragem** head_paragem) {
    char* nomeCarreira,* nomeOrigem,* nomeDestino;
    double custo, duracao;
    Carreira* carreira;
    Paragem* origem,* destino;

    leNomesComando(&nomeCarreira,&nomeOrigem,&nomeDestino,
                   head_carreira,head_paragem);
    scanf("%lf%lf", &custo, &duracao);
    leAteFinalLinha();

    carreira = encontraCarreira(head_carreira,nomeCarreira);
    if (carreira == NULL)
        printf("%s: no such line.\n", nomeCarreira);
    else {
        origem = encontraParagem(head_paragem,nomeOrigem);
        if (origem == NULL)
	    printf("%s: no such stop.\n", nomeOrigem);
	    else {
	    destino = encontraParagem(head_paragem,nomeDestino);
	    if (destino == NULL)
	        printf("%s: no such stop.\n", nomeDestino);
	    else if (custo < 0.0 || duracao < 0.0)
	            printf("negative cost or duration.\n");
		else
		    adicionaLigacao(carreira,origem,destino,custo,duracao,
                            head_carreira,head_paragem);
	    }
    }
    free(nomeCarreira); free(nomeOrigem); free(nomeDestino);
}


/* FUNÇÕES PARA TRATAR AS INTERSEÇÕES */

/* Aplica Bubble Sort a uma lista ligada de carreiras(ids) de uma paragem. */
void ordenaCarreiras(Paragem *paragem, int numCarreiras) {
    int i, houveTroca = VERDADE;
    Carreira *temp;
    Carreira **atual;

    if (numCarreiras == 0) {
        Carreira *current = paragem->head;
        numCarreiras = 0;
        while (current != NULL) {
            numCarreiras++;
            current = current->prox;
        }
    }

    for (i = 0; houveTroca && i < numCarreiras - 1; i++) {
        houveTroca = FALSO;
        atual = &paragem->head;

        while (*atual != NULL && (*atual)->prox != NULL && i < numCarreiras-1) {
            if (strcmp((*atual)->id, (*atual)->prox->id) > 0) {
                temp = *atual;
                *atual = temp->prox;
                temp->prox = (*atual)->prox;
                (*atual)->prox = temp;
                houveTroca = VERDADE;
            }
            atual = &(*atual)->prox;
        }
    }
}

/* Função para tratar o comando 'i'. */
void intersecoes(Paragem** head_paragem) {
    Paragem* paragem;
    Carreira* c;
    int numCarreiras;

    leAteFinalLinha();

    for (paragem=(*head_paragem);paragem!=NULL;paragem=paragem->prox) {
        numCarreiras = paragem->num_carreiras;
        if (numCarreiras > 1) {
            ordenaCarreiras(paragem,numCarreiras);

            printf("%s %d:", paragem->id, numCarreiras);
            for (c=paragem->head; c != NULL;c=c->prox) {
                printf(" %s",c->id);
            }

            printf("\n");
        }
    }
}


/* FUNÇÕES PARA TRATAR A REMOÇÃO DE UMA CARREIRA E SUAS LIGAÇÕES */

/* Função para dar free à carreira */
void freeCarreira(Carreira* carreira, Carreira** head_carreira) {
    Carreira *current = *head_carreira, *prev = NULL;

    /* Percorre a lista ligada até encontrar a carreira a ser removida */
    while (current != NULL && strcmp(current->id, carreira->id) != 0) {
        prev = current;
        current = current->prox;
    }

    /* Se encontrar a carreira, atualiza o ponteiro da lista ligada */
    if (prev == NULL) {
        *head_carreira = current->prox;
    } else {
        prev->prox = current->prox;
    }

    /* Liberta a memória alocada para a carreira */
    free(carreira->id);
    free(carreira);
}

/* Função para dar free à carreira da lista ligada de carreiras das Paragens */
void freeCarreiraParagens(Carreira* carreira, Paragem** head_paragem) {
    Paragem* paragem;
    Carreira** atual,* temp;
    int num_carreiras = 0;

    for (paragem = *head_paragem; paragem != NULL; paragem = paragem->prox) {
        atual = &paragem->head;
        while (*atual != NULL) {
            /* procura pelo nome da carreira na lista de nomes de carrreiras
               que passam na paragem */
            if (!strcmp((*atual)->id, carreira->id)) {
                temp = *atual;
                *atual = (*atual)->prox;
                free(temp->id);
                free(temp);
            } else {
                num_carreiras++;
                atual = &(*atual)->prox;
            }
        } /* atualiza numero de carreiras que passam na paragem */
        paragem->num_carreiras = num_carreiras;
        num_carreiras = 0;
    }
}

/* Função para dar free às ligacoes de uma carreira */
void freeLigacoesCarreira(Carreira* carreira) {
    Ligacao* current = carreira->head,* aux;

    while (current != NULL) {
        aux = current;
        current = current->prox;
        free(aux->origem);
        free(aux->destino);
        free(aux);
    }
}

/* Função para tratar o comando 'r'. */
void removecarreira(Carreira** head_carreira,Paragem** head_paragem) {
    char* nome;
    Carreira* carreira;

    leEspacos();
    nome = leNome(head_carreira,head_paragem);

    carreira = encontraCarreira(head_carreira,nome);
    if (carreira==NULL) {
        printf("%s: no such line.\n",nome);
        return;
    }

    freeLigacoesCarreira(carreira);
    freeCarreiraParagens(carreira,head_paragem);
    freeCarreira(carreira,head_carreira);
    free(nome);
}


/* FUNÇÕES PARA TRATAR A REMOÇÂO DE UMA PARAGEM DO SISTEMA */

/* Função dá free a uma ligação. */
void freeligacao(Ligacao* ligacao) {
    free(ligacao->origem);
    free(ligacao->destino);
    free(ligacao);
}

/* Função dá free a uma carreira da lista de carreiras de uma única Paragem
   Atualiza o numero de carreiras da paragem(-1). */
void freeCarreiraParagem(Carreira* carreira, Paragem* paragem,
                         Paragem** head_paragem) {
    /* Encontra a paragem na lista ligada de paragens */
    Paragem* p = encontraParagem(head_paragem, paragem->id);

    /* Percorre a lista ligada de carreiras da paragem */
    Carreira** current_carreira = &(p->head);
    Carreira* temp;
    while (*current_carreira != NULL) {
        if (!strcmp((*current_carreira)->id, carreira->id)) {
            /* Remove a carreira da lista ligada de carreiras da paragem */
            temp = *current_carreira;
            *current_carreira = (*current_carreira)->prox;
            free(temp->id);
            free(temp);
            /* Atualiza o número de carreiras da paragem */
            p->num_carreiras--;
            break;
        }
        current_carreira = &((*current_carreira)->prox);
    }
}

/* Função que lida com o caso em que paragem a remover está na paragem
   de origem de uma ligacao. */
void caso_origem(Ligacao* current,Carreira** carreira,Paragem** head_paragem) {
    Ligacao* prev=NULL, *next=NULL;
    next = current->prox;
    prev = current->prev;
    /* Atualiza os ponteiros conforme o caso */
    if ((current == (*carreira)->head) && (current == (*carreira)->tail)) {
        (*carreira)->head=NULL;
        (*carreira)->tail=NULL;
    }
    else if (current == (*carreira)->head) {
        (*carreira)->head=next;
        next->prev=NULL;
    }
    else {
        prev->prox = next;
        next->prev = prev;
    }
    /* Atualiza a carreira */
    (*carreira)->num_ligacoes--;
    (*carreira)->custo -= current->custo;
    (*carreira)->duracaoTotal -= current->duracao;

    /* a carreira deixa também de estar associada à paragem de destino */
    if((*carreira)->num_ligacoes == 0)
        freeCarreiraParagem((*carreira),encontraParagem(head_paragem,current->destino),head_paragem);
}

/* Função que lida com o caso em que paragem a remover está na paragem
   de destino de uma ligacao que é a tail. */
void caso_ultimo_destino(Ligacao* current,Carreira** carreira,
                         Paragem** head_paragem) {
    Ligacao* prev=NULL;
    prev = current->prev;
    /* Atualiza os ponteiros conforme o caso */
    if ((current == (*carreira)->head) && (current == (*carreira)->tail)) {
        (*carreira)->head=NULL;
        (*carreira)->tail=NULL;
    }
    else if((*carreira)->tail == current) {
        (*carreira)->tail=prev;
        (*carreira)->tail->prox=NULL;
    }
    /* Atualiza a carreira */
    (*carreira)->num_ligacoes--;
    (*carreira)->custo -= current->custo;
    (*carreira)->duracaoTotal -= current->duracao;

    /* a carreira deixa também de estar associada à paragem de origem */
    if((*carreira)->num_ligacoes == 0)
        freeCarreiraParagem((*carreira),encontraParagem(head_paragem,current->origem),head_paragem);
}

/* Função que lida com o caso em que paragem a remover está na paragem
   de destino de uma ligacao que  não é a tail. */
void caso_destino(Ligacao* current,Carreira** carreira,
                  Carreira** head_c,Paragem** head_p) {
    Ligacao* next=NULL,* next_prox=NULL;
    int len;
    next = current->prox;
    next_prox = current->prox->prox;

    /* Atualiza a ligacao incrementando os valores da próxima */
    current->custo += next->custo;
    current->duracao += next->duracao;
    /* Modifica a paragem de destino da ligacao para o destino da próxima */
    len=strlen(next->destino);
    free(current->destino);
    current->destino = (char*) malloc(sizeof(char)*(len+1));
    is_null(current->destino,head_c,head_p);
    strcpy(current->destino,next->destino);

    freeligacao(next);
    /* Atualiza os ponteiros conforme o caso */
    current->prox = next_prox;
    if (next_prox != NULL) next_prox->prev = current;
    else (*carreira)->tail = current;

    (*carreira)->num_ligacoes--;
}

/* Função para editar as ligações de uma carreira */
void editListaLigacoes(Paragem* paragem, Carreira** carreira,
                       Carreira** head_carreira,Paragem** head_paragem) {
    Ligacao* aux;
    Ligacao* current = (*carreira)->head;
    int acao=0;
    while(current != NULL) {
        acao=0;
        if (!strcmp(current->origem,paragem->id)) {
            caso_origem(current,carreira,head_paragem);
            acao=1;
        }
        else if (!strcmp(current->destino,paragem->id) && current->prox == NULL) {
            caso_ultimo_destino(current,carreira,head_paragem);
            acao=2;
        }
        else if (!strcmp(current->destino,paragem->id) && current->prox != NULL)  {
            caso_destino(current,carreira,head_carreira,head_paragem);
            acao=3;
        }
        /* damos free à paragem->prox, por isso não atualizamos o current */
        if (acao!=3) {
            if (acao!=0) {
                aux = current;
                current = current->prox;
                free(aux);
            }
            else current = current->prox;
        }
    }
}

/* Função vai editar a lista de ligacoes de cada carreira */
void freeParagemLigacoesCarreira(Paragem* paragem,Carreira** head_carreira,
                                 Paragem** head_paragem) {
    Carreira* carreira = (*head_carreira);

    while(carreira != NULL) {
        if (carreira->num_ligacoes>0)
            editListaLigacoes(paragem,&carreira,head_carreira,head_paragem);
        carreira = carreira->prox;
    }
}

/* Função dá free à lista ligada de carreiras de uma paragem */
void freeParagemListaCarreiras(Paragem* paragem) {
    Carreira* current = paragem->head;
    Carreira* next = NULL;

    while (current != NULL) {
        next = current->prox;
        free(current->id);
        free(current);
        current = next;
    }
}

/* Função dá free à paragem e à sua lista de carreiras */
void freeParagem(Paragem* paragem, Paragem** head_paragem) {
    Paragem* current = *head_paragem, *prev = NULL;

    /* Percorre a lista ligada até encontrar a paragem a ser removida */
    while (current != NULL && strcmp(current->id, paragem->id) != 0) {
        prev = current;
        current = current->prox;
    }

    /* Se encontrar a paragem, atualiza o ponteiro da lista ligada */
    if (prev == NULL) {
        *head_paragem = current->prox;
    } else {
        prev->prox = current->prox;
    }

    /* Liberta a memória alocada para a paragem */
    freeParagemListaCarreiras(paragem);
    free(paragem->id);
    free(paragem);
}

/* Função para tratar o comando 'e'. */
void removeparagem(Carreira** head_carreira,Paragem** head_paragem) {
    char* nome;
    Paragem* paragem;
    leEspacos();
    nome = leNome(head_carreira,head_paragem);

    paragem = encontraParagem(head_paragem,nome);
    if (paragem==NULL) {
        printf("%s: no such stop.\n",nome);
        free(nome);
        return;
    }
    freeParagemLigacoesCarreira(paragem,head_carreira,head_paragem);
    freeParagem(paragem,head_paragem);
    free(nome);
}


/* FUNÇÃO PARA DAR FREE A TUDO */
void apagaMemoria(Carreira** head_carreira,Paragem** head_paragem) {
    Paragem* current_p = (*head_paragem),* aux_p;
    Carreira* current_c = (*head_carreira),* aux_c;

    while(current_p!=NULL) {
        aux_p = current_p;
        current_p = current_p->prox;
        freeParagem(aux_p,head_paragem);
    }
    while(current_c!=NULL) {
        aux_c = current_c;
        current_c = current_c->prox;
        freeLigacoesCarreira(aux_c);
        freeCarreira(aux_c,head_carreira);
    }
}

/* FUNÇÃO MAIN */
int main() {
    int c;
    Carreira* head_carreira = NULL;
    Paragem* head_paragem = NULL;

    do {
        c = getchar();
        switch(c) {
            case 'q':
                apagaMemoria(&head_carreira,&head_paragem);
	            break;
            case 'c':
                carreiras(&head_carreira,&head_paragem);
                break;
            case 'p':
                paragens(&head_paragem,&head_carreira);
                break;
            case 'l':
                ligacoes(&head_carreira,&head_paragem);
                break;
            case 'i':
                intersecoes(&head_paragem);
                break;
	        case 'r':
                removecarreira(&head_carreira,&head_paragem);
	            break;
            case 'e':
                removeparagem(&head_carreira,&head_paragem);
                break;
            case 'a':
                apagaMemoria(&head_carreira,&head_paragem);
                break;
            default:
	        /* Ignorar linhas em branco */
	        if (c == ' ' || c == '\t' || c == '\n') break;
        }
    } while (c != 'q');
    return 0;
}
