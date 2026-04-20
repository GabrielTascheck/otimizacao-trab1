#include <stdio.h>
#include <stdlib.h>
// Entrada: Inicia com 2 números inteiros c e p indicando a quantidade de
// componentes e a quantidade de tipos de comprimidos, respectivamente.
// Em seguida temos uma linha com c números inteiros indicando as quan-
// tidades diárias necessárias de cada componente, q1, q2, . . . , qc.
// Em seguida temos p linhas, cada uma com os dados de um tipo de
// comprimido. Cada comprimido i é descrito com c + 1 números inteiros,
// sendo ri,1, ri,2, . . . , ri,c as quantidades de cada componente presentes no
// comprimido e vi o seu preço, com i = 1, 2, . . . , p.
// 2
// Em seguida é dado um número inteiro k indicando a quantidade de com-
// ponentes limitados, seguido de k linhas com 2 números inteiros cada,
// fi e li, indicando o índice e o limite do i-ésimo componente limitado,
// com i = 1, 2, . . . , k.

/*
Ex entrada

3 2
10 5 0
5 1 0 2
2 4 1 3
1
3 0

3 = Componentes
2 = Comprimidos

10 = qntd diario componente 1
5 = qntd diario c2
0 = qntd diario c3

5 = comprimido 1 tem 5 do componente 1
1 = comprimido 1 tem 1 do componente 2
0 = comprimido 1 tem 0 do componente 3
2 = comprimido 1 tem preço 2

2 4 1 3 = igual mas para comprimido 2

1 = quantos componentes tem limite diario

3 0 -> qual componente tem limite diario (3) e qual o limite (0)

*/

struct limiteComp
{
        size_t componente;
        size_t limite;
};

// Imprime o PL formado a partir dos dados de entrada
int print_PL(size_t c, size_t p, size_t *q, size_t **r, size_t k, struct limiteComp *fl)
{
    fprintf(stdout, "min : ");

    for(size_t i = 0; i < p - 1; i++) {
        fprintf(stdout, "%zux%zu + ", r[i][c], i + 1);
    }
    fprintf(stdout, "%zux%zu;\n", r[p - 1][c], p);

    for(size_t i = 0; i < c; i++) {
        for(size_t j = 0; j < p - 1; j++) {
            fprintf(stdout, "%zux%zu + ", r[j][i], j + 1);
        }
        fprintf(stdout, "%zux%zu >= %zu;\n", r[p - 1][i], p, q[i]);
    }

    for(size_t i = 0; i < k; i++) {
        for(size_t j = 0; j < p - 1; j++) {
            fprintf(stdout, "%zux%zu + ", r[j][fl[i].componente - 1], j + 1);
        }
        fprintf(stdout, "%zux%zu <= %zu;\n", r[p - 1][fl[i].componente - 1], p, fl[i].limite);
    }

    return 0;
}

// Lê os dados de entrada e verifica casos de inviabilidade
void le_dados(size_t *c, size_t *p, size_t **q, size_t ***r, size_t *k, struct limiteComp **fl)
{
    scanf("%zu %zu", c, p);

    *q = malloc(*c * sizeof(size_t));
    // Aloca matriz r dinamicamente
    *r = malloc(*p * sizeof(size_t *));
    (*r)[0] = malloc(*p * (*c + 1) * sizeof(size_t));
    for(size_t i = 1; i < *p; i++) {
        (*r)[i] = (*r)[0] + i * (*c + 1);
    }

    if(*c < 1 || *p < 1) {
        fprintf(stderr, "\nC ou P menores que 1\n");
        exit(1);
    }

    for(size_t i = 0; i < *c; i++) {
        scanf("%zu", &(*q)[i]);
    }

    for(size_t i = 0; i < *p; i++) {
        for(size_t j = 0; j <= *c; j++) {
            scanf("%zu", &(*r)[i][j]);
        }
    }

    scanf("%zu", k);
    *fl = malloc(*k * sizeof(struct limiteComp));

    if(*k > *c) {
        fprintf(stderr, "\nK Maior que C\n");
        exit(1);
    }
    
    size_t limite, componente;

    for(size_t i = 0; i < *k; i++) {
        scanf("%zu %zu", &componente, &limite);
        
        if(componente > *c || componente < 1) {
            fprintf(stderr, "\nComponente inválido\n");
            exit(1);
        }

        if(limite < (*q)[componente - 1]) { // limite < quantidade diária
            fprintf(stderr, "\nLimite inviável\n");
            exit(1);
        }
        

        (*fl)[i].limite = limite;
        (*fl)[i].componente = componente;
    }

    
    // Vendo se algum comprimido tem o x componente
    int flag = 1;
    for(size_t i = 0; i < *c; i++) {
        if((*q)[i] == 0)
            continue;
        flag = 1;
        for(size_t j = 0; j < *p && flag; j++) {
            if((*r)[j][i] > 0 && (*r)[j][i] <= (*q)[i])
            flag = 0;
        }
        if(flag) {
            fprintf(stderr, "\nNão há comprimido que solucione o componente %zu\n", i + 1);
            exit(1);
        }
    }
}

// Imprime tabelas com dados de entrada
void print_dados(size_t c, size_t p, size_t *q, size_t **r, size_t k, struct limiteComp *fl)
{
    // 1. Tabela de Necessidades Diárias [cite: 25, 31]
    printf("=== Necessidades Diárias ===\n");
    printf("%-12s | %-17s\n", "Componente", "Quantidade Diária");
    printf("-------------|------------------\n");
    for(size_t i = 0; i < c; i++) {
        // %-12zu alinha o índice à esquerda com 12 espaços
        // %-17d alinha a quantidade à esquerda com 17 espaços
        printf("C%-11zu | %-17zu\n", i + 1, q[i]);
    }

    // 2. Tabela de Composição dos Comprimidos
    printf("\n=== Tabela de Comprimidos ===\n");

    // Cabeçalho Dinâmico
    printf("%-11s", "Comprimido");
    for(size_t i = 0; i < c; i++) {
        printf(" | Comp. %-2zu", i + 1);
    }
    printf(" | %-7s\n", "Preço");

    // Linha separadora dinâmica (opcional, mas ajuda visualmente)
    printf("------------");
    for(size_t i = 0; i < c; i++)
        printf("+----------");
    printf("+----------\n");

    // Dados dos Comprimidos
    for(size_t i = 0; i < p; i++) {
        printf("%-11zu", i + 1); // Índice do comprimido
        for (size_t j = 0; j < c; j++) {
            printf(" | %-8zu", r[i][j]); // Quantidade de cada componente
        }
        printf(" | %-7zu\n", r[i][c]); // Preço (última coluna) [cite: 33]
    }

    // 3. Tabela de Restrições (Componentes Limitados) [cite: 25, 35]
    printf("\n=== Componentes Limitados ===\n");
    printf("%-12s | %-10s\n", "Índice (f)", "Limite (l)");
    printf("------------|------------\n");
    for(size_t i = 0; i < k; i++) {
        printf("C%-10zu | %-10zu\n", fl[i].componente, fl[i].limite);
    }
}

int main()
{
    size_t c, p;           // # Componentes / # Comprimidos
    size_t *q;             // Quantidade diária para cada componente
    size_t **r;            // Quanto cada remedio tem de cada componente
    size_t k;              // qntd componentes limitados
    struct limiteComp *fl; // Qual componente e qual seu limite

    le_dados(&c, &p, &q, &r, &k, &fl);

    // print_dados(c, p, q, r, k, fl);

    print_PL(c, p, q, r, k, fl);
    

    free(r[0]);
    free(r);
    free(q);
    free(fl);

    return 0;
}
