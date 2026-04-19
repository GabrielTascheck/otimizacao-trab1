#include <stdio.h>
#include <stdlib.h>
// Entrada: Inicia com 2 n´umeros inteiros c e p indicando a quantidade de
// componentes e a quantidade de tipos de comprimidos, respectivamente.
// Em seguida temos uma linha com c n´umeros inteiros indicando as quan-
// tidades di´arias necess´arias de cada componente, q1, q2, . . . , qc.
// Em seguida temos p linhas, cada uma com os dados de um tipo de
// comprimido. Cada comprimido i ´e descrito com c + 1 n´umeros inteiros,
// sendo ri,1, ri,2, . . . , ri,c as quantidades de cada componente presentes no
// comprimido e vi o seu pre¸co, com i = 1, 2, . . . , p.
// 2
// Em seguida ´e dado um n´umero inteiro k indicando a quantidade de com-
// ponentes limitados, seguido de k linhas com 2 n´umeros inteiros cada,
// fi e li, indicando o ´ındice e o limite do i-´esimo componente limitado,
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

// Colocar em outro arquivo!?
struct limiteComp
{
        int componente;
        int limite;
};

int main()
{

        int c, p; // # Componentes / # Comprimidos
        scanf("%d %d", &c, &p);

        
        if (c < 1 || p < 1)
        {
                fprintf(stderr, "\nC ou P menores que 1");
                return 1;
        }
        

        int q[c]; // Quantidade diária para cada componente

        for (size_t i = 0; i < c; i++)
        {
                scanf("%d", &q[i]);
                
                if (q[i] < 0)
                {
                        fprintf(stderr, "\nQuantidade diária negativa");
                        return 1;
                }
                
        }

        // Usar ponteiro?
        int r[p][c + 1]; // Quanto cada remedio tem de cada componente
        for (size_t i = 0; i < p; i++)
        {
                for (size_t j = 0; j <= c; j++)
                {
                        scanf("%d", &r[i][j]);
                        
                        if (r[i][j] < 0)
                        {
                                fprintf(stderr, "\nComprimido tem quantidade negativa de componente");
                                return 1;
                        }
                        
                }
        }

        int k; // qntd componentes limitados
        scanf("%d", &k);
        

        if (k > c)
        {
                fprintf(stderr, "\nK Maior que C");
                return 1;
        }
        

        struct limiteComp fl[k]; // Qual componente e qual seu limite

        for (size_t i = 0; i < k; i++)
        {
                int limite, componente;
                scanf("%d %d", &componente, &limite);
                
                if (componente > c || componente < 1)
                {
                        fprintf(stderr, "\nComponente inválido");
                        return 1;
                }

                if (limite < q[componente - 1]) // limite < quantidade diária
                {
                        fprintf(stderr, "\nLimite inviável");
                        return 1;
                }
                

                fl[i].limite = limite;
                fl[i].componente = componente;
        }

        // Não sei como verificar se existe solução para uma certa entrada!!

        
        // Vendo se algum comprimido tem o x componente
        for (size_t i = 0; i < c; i++)
        {
                if (q[i] == 0)
                continue;
                int flag = 1;
                for (size_t j = 0; j < p && flag; j++)
                {
                        if (r[j][i] > 0 && r[j][i] <= q[i])
                        flag = 0;
                }
                if (flag)
                {
                        fprintf(stderr, "\nNão há comprimido que solucione o componente %zu", i + 1);
                        return 1;
                }
        }
        

        /*==================================================================================*/
        // Vendo se os dados entraram certo

        // 1. Tabela de Necessidades Diárias [cite: 25, 31]
        printf("\n=== Necessidades Diárias ===\n");
        printf("%-12s | %-17s\n", "Componente", "Quantidade Diária");
        printf("-------------|------------------\n");
        for (size_t i = 0; i < c; i++)
        {
                // %-12zu alinha o índice à esquerda com 12 espaços
                // %-17d alinha a quantidade à esquerda com 17 espaços
                printf("C%-11zu | %-17d\n", i + 1, q[i]);
        }

        // 2. Tabela de Composição dos Comprimidos
        printf("\n=== Tabela de Comprimidos ===\n");

        // Cabeçalho Dinâmico
        printf("%-11s", "Comprimido");
        for (size_t i = 0; i < c; i++)
        {
                printf(" | Comp. %-2zu", i + 1);
        }
        printf(" | %-7s\n", "Preço");

        // Linha separadora dinâmica (opcional, mas ajuda visualmente)
        printf("------------");
        for (size_t i = 0; i < c; i++)
                printf("+----------");
        printf("+----------\n");

        // Dados dos Comprimidos
        for (size_t i = 0; i < p; i++)
        {
                printf("%-11zu", i + 1); // Índice do comprimido
                for (size_t j = 0; j < c; j++)
                {
                        printf(" | %-8d", r[i][j]); // Quantidade de cada componente
                }
                printf(" | %-7d\n", r[i][c]); // Preço (última coluna) [cite: 33]
        }

        // 3. Tabela de Restrições (Componentes Limitados) [cite: 25, 35]
        printf("\n=== Componentes Limitados ===\n");
        printf("%-12s | %-10s\n", "Índice (f)", "Limite (l)");
        printf("------------|------------\n");
        for (size_t i = 0; i < k; i++)
        {
                printf("C%-10d | %-10d\n", fl[i].componente, fl[i].limite);
        }
        return 0;
}