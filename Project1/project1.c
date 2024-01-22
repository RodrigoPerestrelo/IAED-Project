/* iaed-23 - ist1106074 - project1 */

/*
 * Ficheiro: project1.c
 * Autor: Rodrigo Salvador dos Santos Perestrelo, ist1106074
 * Descricao: Este programa visa dar uma solucao ao problema/exercicio exposto
no enunciado(projeto1).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Espaco necessario para \0 */
#define NULL_TERMINATOR 1

/* Numero maximo de carateres que o id de uma carreira pode ter */
#define ID_CARR_MAX 20

/* Numero maximo de carateres que o id de uma paragem pode ter */
#define ID_PRGM_MAX 50

/* Numero maximo de carreiras que podem existir */
#define MAX_CARREIRAS 200

/* Numero maximo de paragens que podem existir */
#define MAX_PARAGENS 10000

/* Numero maximo de ligacoes que podem existir */
#define MAX_LIGACOES 30000


/* Flag para dar entrada/continuar dentro do loop */
#define INLOOP 1

/* Flag para sair do loop */
#define OUTLOOP 0

/* Numero maximo de bytes que uma linha de comandos pode ter */
#define BUFSIZE 8192


/* Estruturas */

/* Estrutura Paragem */
typedef struct Paragem {
    char id[ID_PRGM_MAX + NULL_TERMINATOR];
    double latitude;
    double longitude;
} Paragem;

/* Estrutura Carreira */
typedef struct Carreira {
    char id[ID_CARR_MAX + NULL_TERMINATOR];
    Paragem origem;
    Paragem destino;
    int num_paragens;
    double custo;
    double duracaoTotal;

} Carreira;

/* Estrutura Ligacao */
typedef struct Ligacao {
    Carreira carreira;
    Paragem origem;
    Paragem destino;
    double custo;
    double duracao;
} Ligacao;


/* Variaveis Globais */
Paragem paragens[MAX_PARAGENS]; /* vetor para armazenar as paragens criadas */
int cont_paragens=0; /* contador do numero de paragens criadas/armazenadas */

/* vetor para armazenar as carreiras criadas */
Carreira carreiras[MAX_CARREIRAS];
int cont_carreiras=0; /* contador do numero de carreiras criadas/armazenadas */

Ligacao ligacoes[MAX_LIGACOES]; /* vetor para armazenar as ligacoes criadas */
int cont_ligacoes=0; /* contador do numero de carreiras criadas/armazenadas */


/* Funcoes Auxliares */

/* A funcao avaliaCarreirasParagem retorna o numero de carreiras que param numa
paragem (cujo indice é fornecido) */
int avaliaCarreirasParagem(int i) {
    int j,k,num_carreiras=0;
    /* Vetor auxiliar para guardar as carreiras encontradas */
    Carreira aux[MAX_CARREIRAS];

    for (j=0;j<cont_ligacoes;j++) {
        /* Verifica se a paragem em questao é  origem ou destino da ligação */
        if (!strcmp(paragens[i].id, ligacoes[j].origem.id) ||
            !strcmp(paragens[i].id, ligacoes[j].destino.id)) {
            k = 0;
            /*Loop para verificar se a carreira já está presente no vetor aux*/
            while (k<num_carreiras &&
            strcmp(ligacoes[j].carreira.id, aux[k].id)) {
                k++;
            }
            /*Adiciona a carreira no vetor aux se a mesma nao existir no vetor*/
            if (k==num_carreiras) {
                strcpy(aux[num_carreiras].id, ligacoes[j].carreira.id);
                num_carreiras++;
            }
        }
    }
    return num_carreiras;
}


/* A funcao imprimeParagens escreve todas as paragens existentes */
void imprimeParagens() {
    int i;

    for (i=0;i<cont_paragens;i++) {
        printf("%s: %16.12f %16.12f %d\n",paragens[i].id,paragens[i].latitude,
        paragens[i].longitude,avaliaCarreirasParagem(i));
    }
}


/* A funcao verificaParagem verifica se uma determinada paragem existe,
caso exista retorna o seu indice */
int verificaParagem(char id_p[]) {
    int i;

    for (i=0;i<cont_paragens;i++) {
        if (!strcmp(paragens[i].id,id_p))
            return i;
    }
    return -1; /* retorna -1 se a paragem não existir */
}


/* A funcao escreve todas as paragens de uma carreira, pela ordem desejada */
void imprimeParagensCarreira(int i,int inverso) {
    int j, num_paragens;
    /* Array de strings para armazenar os nomes das paragens */
    char paragens[MAX_PARAGENS][ID_PRGM_MAX+NULL_TERMINATOR];
    int num_paragens_guardadas = 0; /* Contador de paragens guardadas */
    num_paragens = carreiras[i].num_paragens;

    /* Inicializa o número de paragens e adiciona a paragem de origem à lista */
    strcpy(paragens[num_paragens_guardadas], carreiras[i].origem.id);
    num_paragens_guardadas++;

    /*Itera sobre todas as ligações da carreira e adiciona as paragens à lista*/
    for (j = 0; j < cont_ligacoes; j++) {
        if (strcmp(ligacoes[j].carreira.id, carreiras[i].id) == 0) {
            if(!strcmp(ligacoes[j].origem.id,paragens[num_paragens_guardadas-1])
                && num_paragens > 1) {
                strcpy(paragens[num_paragens_guardadas],ligacoes[j].destino.id);
                num_paragens_guardadas++;
                /* Reinicia o loop para verificar se a nova paragem é a origem
                de uma nova ligação */
                j = -1;
                num_paragens--;
            }
            if (num_paragens == 1) {
                break;
            }
        }
    }
    /* Verifica se a carreira tinha paragens associadas*/
    if (num_paragens_guardadas==1) {
        return;
    }
    /* Imprime as paragens na ordem desejada (normal ou inversa) */
    if (inverso==0) {
        printf("%s",paragens[0]);
        for (j=1;j<num_paragens_guardadas;j++) {
            printf(", %s",paragens[j]);
        }
        printf("\n");
    }
    else {
        printf("%s",paragens[num_paragens_guardadas-1]);
        for (j=num_paragens_guardadas-2;j>=0;j--) {
            printf(", %s",paragens[j]);
        }
        printf("\n");
    }
}


/* A funcao imprimeCarreiras escreve todas as carreiras existentes */
void imprimeCarreiras() {
    int i;

    for (i=0;i<cont_carreiras;i++){
        if (carreiras[i].num_paragens == 0)
            printf("%s %d %.2f %.2f\n",
            carreiras[i].id,carreiras[i].num_paragens,carreiras[i].custo,
            carreiras[i].duracaoTotal);
        else {
            printf("%s %s %s %d %.2f %.2f\n",carreiras[i].id,
            carreiras[i].origem.id,carreiras[i].destino.id,
            carreiras[i].num_paragens,carreiras[i].custo,
            carreiras[i].duracaoTotal);
        }
    }
}


/* A funcao verificaCarreira verifica se uma determinada carreira existe,
caso exista retorna o seu indice */
int verificaCarreira(char id_c[]) {
    int i;

    for (i=0;i<cont_carreiras;i++){
        if (!strcmp(carreiras[i].id,id_c))
            return i;
    }
    return -1; /* retorna -1 se a carreira não existir */
}


/* A funcao c_decision e a funcao-mae do comando c, esta funcao decide as acoes
a realizar consoante o input fornecido */
void c_decision(int option,char id_c[]) {
    int i,inverso=0;

    switch(option) {
        /* O comando c sem argumentos permite listar todas as carreiras */
        case -1:
            imprimeCarreiras();
            break;
        /* Seguido de uma carreira existente permite listar todas as paragens
        da carreira (origem-destino) */
        case 1:
            i = verificaCarreira(id_c);
            if (i != -1) {
                inverso = 0;
                imprimeParagensCarreira(i,inverso);
                break;
            /* Seguido de uma carreira não existente no sistema,
            permite criar a nova carreira */
            } else {
                strcpy(carreiras[cont_carreiras].id, id_c);
                cont_carreiras++;
                break;
            }
        /* Abreviacao de inverso nao valida */
        case 2:
            printf("incorrect sort option.\n");
            break;
        /* Se possuir uma abreviacao de inverso valida,
        as paragens são listadas do destino para a origem */
        case 3:
            i = verificaCarreira(id_c);
            if (i!=-1) {
                inverso = 1;
                imprimeParagensCarreira(i,inverso);
                break;
            }
    }
}


/* A funcao p_decision e a funcao-mae do comando p, esta funcao decide as acoes
a realizar consoante o input fornecido */
void p_decision(int option,char id_p[],double latitude,double longitude){
    int i;

    switch(option) {
        /* O comando p sem argumentos permite listar todas as paragens */
        case -1:
            imprimeParagens();
            break;
        /* Com um argumento mostra a latitude e longitude da paragem */
        case 1:
            i = verificaParagem(id_p);
            if (i != -1) {
                printf("%16.12f %16.12f\n",
                paragens[i].latitude,paragens[i].longitude);
                break;
            }
            else { /* Se a paragem nao existir */
                printf("%s: no such stop.\n", id_p);
                break;
            }
        /* O comando p com três argumentos cria uma nova paragem */
        case 3:
            i = verificaParagem(id_p);
            if (i == -1) {
                strcpy(paragens[cont_paragens].id, id_p);
                paragens[cont_paragens].latitude = latitude;
                paragens[cont_paragens].longitude = longitude;
                cont_paragens++;
                break;
            } else { /* Se a paragem ja existir */
                printf("%s: stop already exists.\n", id_p);
                break;
            }
    }
}


/* A funcao l_decision e a funcao-mae do comando l, esta funcao decide as acoes
a realizar consoante o input fornecido */
int l_decision(char id_c[],char origem[],char destino[],
double custo,double duracao) {
    int i=0;

    if (verificaCarreira(id_c) == (-1)) { /* Verifica se a carreira existe */
        printf("%s: no such line.\n",id_c);
        return 1;
    }
    i = verificaCarreira(id_c); /* Se existe, guarda o indice da carreira */

    /* Verifica se a paragem de origem existe */
    if (verificaParagem(origem) == (-1)) {
        printf("%s: no such stop.\n",origem);
        return 1;
    }

    /* Verifica se a paragem de destino existe */
    if (verificaParagem(destino) == (-1)) {
        printf("%s: no such stop.\n",destino);
        return 1;
    }

    /* Verifica se o custo e a duração sao positivos */
    if ((custo<0) || (duracao<0)) {
        printf("negative cost or duration.\n");
        return 1;
    }

    /* Adiciona a nova ligacao as ligacoes, mas ainda nao de forma definitiva */
    strcpy(ligacoes[cont_ligacoes].carreira.id, id_c);
    strcpy(ligacoes[cont_ligacoes].origem.id, origem);
    strcpy(ligacoes[cont_ligacoes].destino.id, destino);
    ligacoes[cont_ligacoes].custo = custo;
    ligacoes[cont_ligacoes].duracao = duracao;

    /* Atualiza a carreira correspondente */

    /* Carreira ainda não tem paragens, a origem e o destino são adicionados */
    if(carreiras[i].num_paragens == 0) {
        strcpy(carreiras[i].origem.id, origem);
        strcpy(carreiras[i].destino.id, destino);
        carreiras[i].num_paragens+=2;
        carreiras[i].custo+= custo;
        carreiras[i].duracaoTotal+= duracao;
        cont_ligacoes++; /* Adiciona a nova ligacao de forma definitiva */
    }
    /* Liga o destino da carreira a origem da nova ligacao */
    else if (!strcmp(carreiras[i].destino.id, origem)) {
        strcpy(carreiras[i].destino.id, destino);
        carreiras[i].num_paragens+=1;
        carreiras[i].custo+= custo;
        carreiras[i].duracaoTotal+= duracao;
        cont_ligacoes++; /* Adiciona a nova ligacao de forma definitiva */
    }
    /* Liga a origem da carreira ao destino da nova ligacao */
    else if (!strcmp(carreiras[i].origem.id, destino)) {
        strcpy(carreiras[i].origem.id, origem);
        carreiras[i].num_paragens+=1;
        carreiras[i].custo+= custo;
        carreiras[i].duracaoTotal+= duracao;
        cont_ligacoes++; /* Adiciona a nova ligacao de forma definitiva */
    }
    /* A ligacao nao e valida */
    else {
        printf("link cannot be associated with bus line.\n");
        return 1;
    }

    return 0;
}


/* A funcao bubble_sort faz um bubble_sort com o nome das carreiras */
void bubble_sort(Carreira arr[],int n) {
    int i, j;
    Carreira temp;

    for (i=0;i<n-1;i++) {
        for (j=0;j<n-i-1;j++) {
            if (strcmp(arr[j].id,arr[j+1].id) > 0) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}


/* A funcao carreirasInterParagem permite encontrar, ordenar e escrever
as paragens que correspondem a intersecoes de carreiras */
void carreirasInterParagem(int i) {
    int j,k=0,num_carreiras=0;
    /* Array de struct Carreira, armazena as carreiras que passam na paragem */
    Carreira aux[MAX_CARREIRAS];

    for (j=0;j<cont_ligacoes;j++) {
        if (!strcmp(paragens[i].id,ligacoes[j].origem.id) ||
            !strcmp(paragens[i].id,ligacoes[j].destino.id)) {
            k = 0;
            /* Verifica se a carreira já foi guardada no array auxiliar */
            while (k<num_carreiras &&
            strcmp(ligacoes[j].carreira.id, aux[k].id)) {
                k++;
            }
            /* Se não tiver sido guardada, adiciona-a */
            if (k == num_carreiras) {
                strcpy(aux[num_carreiras].id,ligacoes[j].carreira.id);
                num_carreiras++;
            }
        }
    }
    /* Ordena as carreiras alfabeticamente utilizando o algoritmo bubble sort */
    bubble_sort(aux,num_carreiras);
    /* Escreve as carreiras encontradas na paragem */
    for (j=0;j<num_carreiras;j++) {
        printf(" %s",aux[j].id);
    }
}


/* A funcao i_decision e a funcao-mae do comando i, esta funcao lista
as paragens que correspondem a intersecoes de carreiras.*/
void i_decision() {
    int i;
    
    for (i=0;i<cont_paragens;i++) {
        /* Se mais do que uma carreira parar na paragem */
        if (avaliaCarreirasParagem(i)>1) {
            printf("%s %d:",paragens[i].id,avaliaCarreirasParagem(i));
            /* Escreve as carreiras que passam por essa paragem */
            carreirasInterParagem(i);
            printf("\n");
        }
    }
}


/* A funcao avaliaInverso avalia se a string fornecida
é uma abreviacao valida de inverso */
int avaliaInverso(char inverso[]) {
    if ((strcmp("inverso",inverso)==0) || (strcmp("invers",inverso)==0) ||
    (strcmp("inver",inverso)==0) || (strcmp("inve",inverso)==0) ||
    (strcmp("inv",inverso)==0))
        return 1;
    else
        return 0;
}


/* A funcao leinput_c avalia o input fornecido para o comando c e retorna
o numero de argumentos lido */
int leinput_c(char input[],char id_c[]) {
    char inverso[BUFSIZE+NULL_TERMINATOR];
    int cont_c_option;
    inverso[0] = '\0';

    cont_c_option = sscanf(input, "%s %s", id_c, inverso);
    /* Le a string entre aspas, caso o argumento esteja entre aspas */
    if (id_c[0] == '"') {
        cont_c_option = sscanf(input, " \"%[^\"]\" %s", id_c, inverso);
    }
    /* devolve 3 se, de facto, inverso for valido */
    if (avaliaInverso(inverso) == 1)
        cont_c_option++;
    
    return cont_c_option;
}


/* A funcao leinput_p avalia o input fornecido para o comando p e retorna
o numero de argumentos lido */
int leinput_p(char input[],char id_p[],double*latitude,double*longitude) {
    int cont_p_option;

    cont_p_option = sscanf(input, "%s %lf %lf", id_p, latitude, longitude);
    /* Le a string entre aspas, caso o argumento esteja entre aspas */
    if (id_p[0] == '"') {
        cont_p_option = sscanf(input, " \"%[^\"]\" %lf %lf",
        id_p,latitude,longitude);
    }

    return cont_p_option;
}


/* A funcao leinput_l avalia o input fornecido para o comando l e retorna
o numero de argumentos lido */
int leinput_l(char input[], char id_c[], char origem[], char destino[],
double*custo, double*duracao) {
    int cont_l_option=0;

    /* Le as strings entre aspas, caso os argumentos estejam entre aspas,
    das diversas formas possiveis */
    cont_l_option = sscanf(input, " %s \"%[^\"]\" \"%[^\"]\" %lf %lf",
    id_c, origem, destino, custo, duracao);
    if (cont_l_option == 5) {
        return cont_l_option;
    }

    cont_l_option = sscanf(input, " %s \"%[^\"]\" %s %lf %lf",
    id_c, origem, destino, custo, duracao);
    if (cont_l_option == 5) {
            return cont_l_option;
    }

    cont_l_option = sscanf(input, " %s %s \"%[^\"]\" %lf %lf",
    id_c, origem, destino, custo, duracao);
    if (cont_l_option == 5) {
            return cont_l_option;
    }

    else {
        cont_l_option = sscanf(input, "%s %s %s %lf %lf",
        id_c, origem, destino, custo, duracao);
        return cont_l_option;
    }
}


/* Funcao principal (main), consoante o input fornecido,
toma as devidas decisoes */
int main(void) {
    char comando;
    char input[BUFSIZE+NULL_TERMINATOR];
    int option=0;

    double latitude,longitude;
    char id_p[ID_PRGM_MAX+NULL_TERMINATOR];

    char origem[ID_PRGM_MAX+NULL_TERMINATOR];
    char destino[ID_PRGM_MAX+NULL_TERMINATOR];
    double custo,duracao;
    char id_c[ID_CARR_MAX+NULL_TERMINATOR];

    /* O loop principal do programa continua ate que o usuário deseje sair */
    /* A maioria dos comandos tem uma funcao para avaliar o input fornecido
    e uma para tomar a decisão adequada com base no input */
    while(INLOOP) {
        scanf(" %c",&comando);
        fgets(input,BUFSIZE+NULL_TERMINATOR,stdin);
        switch(comando) {
            case 'q': /* Caso o comando seja "q", o programa é encerrado. */
                exit(OUTLOOP);
            case 'c':
                option = leinput_c(input,id_c);
                if (cont_carreiras<=MAX_CARREIRAS)
                    c_decision(option,id_c);
                break;
            case 'p':
                option = leinput_p(input,id_p,&latitude,&longitude);
                if (cont_paragens<=MAX_PARAGENS)
                    p_decision(option,id_p,latitude,longitude);
                break;
            case 'l':
                option = leinput_l(input,id_c,origem,destino,&custo,&duracao);
                if ((option == 5) && (cont_ligacoes<=MAX_LIGACOES))
                    l_decision(id_c,origem,destino,custo,duracao);
                break;
            case 'i':
                i_decision();
                break;
            default:
                if (feof(stdin))
                    exit(OUTLOOP);
        }
    }
    return(0);
}
