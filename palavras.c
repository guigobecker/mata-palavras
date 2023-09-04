//Rodrigo Schmidt Becker

#include "tela.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    float x;
    float y;
} ponto_t;

typedef struct
{
    ponto_t sup_esq;
    ponto_t centro;
    ponto_t inf_dir;
} botao_t;

typedef struct
{
    float x;
    float y;
    float vx;
    float vy;
    int tam;
    int cor;
    char t[30];
    bool invalida;
} palavra_t;

typedef struct
{
    float vx_max;
    float vx_min;
    float vy_max;
    float vy_min;
    int palavras_min;
    int palavras_max;
    char atual[];
} nivel_t;

typedef struct
{
    char palavra[30];
} vetor_t;

typedef struct
{
    int tela_atual;
    int n_palavras;
    int total;
    int pontos;
    int saiu;
    botao_t *botoes;
    palavra_t *palavras;
    nivel_t *nivel;
    vetor_t *todas;
    int sel;
    bool selecionou;
    bool fim;
} jogo_t;

void inicializa_jogo(jogo_t *j);
void desenha_tela(jogo_t *j);
void move_palavras(jogo_t *j);
void verifica_entradas(jogo_t *j);

int main()
{
    jogo_t j;
    inicializa_jogo(&j);

    tela_inicio(500, 500, "Mata-palavras");

    srand(time(NULL));

    while(!j.fim)
    {
        desenha_tela(&j);
        move_palavras(&j);
        verifica_entradas(&j);
    }

    tela_fim();
    return 0;
}

int aleatorio(int m, int M)
{
    return rand()%(M-m+1)+m;
}

void inicializa_jogo(jogo_t *j)
{  
    //aloca memória

    j->palavras = malloc(25 * sizeof(palavra_t));
    j->nivel = malloc(sizeof(nivel_t));
    j->botoes = malloc(4 * sizeof(botao_t));
    j->todas = malloc(500 * sizeof(vetor_t));
    j->sel = -1;
    j->pontos = 0;
    j->saiu = 0;

    //indica em que tela o jogador está

    j->tela_atual = 0;

    //inicializa os botões do menu inicial

    for(int i = 0; i < 3; i++)
    {
        botao_t *b = &(j->botoes[i]);
        b->sup_esq.x = 150;
        b->sup_esq.y = 110 + 100*i;
        b->centro.x = 250;
        b->centro.y = 150 + 100*i;
        b->inf_dir.x = 350;
        b->inf_dir.y = 190 + 100*i;
    }

    //inicializa os botões do menu entre as jogadas

    botao_t *b = &(j->botoes[3]);
    b->sup_esq.x = 150;
    b->sup_esq.y = 210;

    b->centro.x = 250;
    b->centro.y = 250;

    b->inf_dir.x = 350;
    b->inf_dir.y = 290;

    //necessário para que o while na main funcione

    j->fim = false;
}

void move_palavra(jogo_t *j, int i)
{
        palavra_t *p = &(j->palavras[i]);
        p->x += p->vx * SEGUNDOS_POR_QUADRO;
        p->y += p->vy * SEGUNDOS_POR_QUADRO;
    
        //se é a primeira vez que a palavra foi detectada fora da tela, conta que saiu
        
        if(j->palavras[i].invalida == false && (j->palavras[i].x < 0 || 
           j->palavras[i].x > 500 || j->palavras[i].y > 500))
        {

            j->palavras[i].invalida = true;
            j->saiu += 1;

            //se era a palavra selecionada, desseleciona

            if(i == j->sel)
            {
                j->sel = -1;
                j->palavras[i].cor = branco;
            }
        }
        
}

void move_palavras(jogo_t *j)
{
    switch(j->tela_atual)
    {
        case 1:
            for(int i = 0; i < j->n_palavras; i++)
            {
                move_palavra(j, i);
            }
            break;
        case 2:
            break;
    }
}

void desenha_palavra(palavra_t p)
{
    tela_texto(p.x, p.y, p.tam, p.cor, p.t);
}

void desenha_botao(botao_t b, char t[])
{
    tela_retangulo(b.sup_esq.x, b.sup_esq.y, b.inf_dir.x, b.inf_dir.y, 0, branco, branco);
    tela_texto(b.centro.x, b.centro.y, 30, preto, t);
}

void desenha_tela(jogo_t *j)
{
    switch(j->tela_atual)
    {
        //desenha os botões

        case 0:
            desenha_botao(j->botoes[0], "FÁCIL");
            desenha_botao(j->botoes[1], "MÉDIO");
            desenha_botao(j->botoes[2], "DIFÍCIL");
            break;

        //desenha as palavras
        
        case 1:
            for(int i = 0; i < j->n_palavras; i++)
            {
                desenha_palavra(j->palavras[i]);
            }
            break;

        //desenha a tela entre uma jogada e outra

        case 2:
            desenha_botao(j->botoes[3], "CONTINUAR");
            break;
    }

    tela_atualiza();
}

void inicializa_palavras(jogo_t *j)
{
    //inicializa as palavras

    j->n_palavras = aleatorio(j->nivel->palavras_min, j->nivel->palavras_max);

    for(int i = 0; i < j->n_palavras; i++)
    {
        palavra_t *p = &(j->palavras[i]);
        p->x = aleatorio(100, 400);
        p->y = aleatorio(-280, -10);
        p->vx = aleatorio(j->nivel->vx_min, j->nivel->vx_max);
        p->vy = aleatorio(j->nivel->vy_min, j->nivel->vy_max);
        p->tam = aleatorio(15, 25);
        p->cor = branco;
        p->invalida = false;
    }

    //preenche um vetor com todas as palavras do arquivo

    FILE *arq;
    arq = fopen(j->nivel->atual, "r");
    if(arq == NULL)
    {
        printf("Não foi possível abrir o arquivo\n");
        return;
    }
    for(int m = 0; m < 501; m++)
    {
        for(int k = 0; k < 30; k++)
        {
            char c;
            fscanf(arq, "%c", &c);
            if(feof(arq))
            {
                break;
            }
            if(c != '\n')
            {
                j->todas[m].palavra[k] = c;
            }
            if(c == '\n')
            {
                break;
            }
        }
    }
    fclose(arq);

    for(int w = 0; w < j->n_palavras; w++)
    {
        strcpy(j->palavras[w].t, j->todas[aleatorio(0, j->total)].palavra);
    }

    //altera a tela atual

    j->tela_atual = 1;
}

void verifica_entradas(jogo_t *j)
{
    switch(j->tela_atual)
    {
        //verifica se algum botão foi clicado

        case 0:
            if (tela_rato_clicado())
            {
                int mouse_x = tela_rato_x_clique();
                int mouse_y = tela_rato_y_clique();

                if (mouse_x >= 150 && mouse_x <= 350)
                {
                    if (mouse_y >= 110 && mouse_y <= 190)
                    {
                        //seleciona o arquivo com as palavras do nível

                        char a[] = "p1.txt";
                        strcpy(j->nivel->atual, a);

                        //inicializa os limites de velocidade do nível

                        j->nivel->vx_max = 10;
                        j->nivel->vx_min = -10;

                        j->nivel->vy_max = 40;
                        j->nivel->vy_min = 10;

                        j->nivel->palavras_max = 10;
                        j->nivel->palavras_min = 5;

                        //número total de palavras no vetor com todas as palavras do arquivo

                        j->total = 50;

                        //chama a função que inicializa as palavras (a função está fora do loop da main)

                        inicializa_palavras(j);
                    }
                    else if (mouse_y >= 210 && mouse_y <= 290)
                    {
                        char b[] = "p2.txt";
                        strcpy(j->nivel->atual, b);

                        j->nivel->vx_max = 7.5;
                        j->nivel->vx_min = -7.5;

                        j->nivel->vy_max = 45;
                        j->nivel->vy_min = 15;

                        j->nivel->palavras_max = 15;
                        j->nivel->palavras_min = 10;

                        j->total = 150;

                        inicializa_palavras(j);
                    }
                    else if (mouse_y >= 310 && mouse_y <= 390)
                    {
                        char c[] = "p3.txt";
                        strcpy(j->nivel->atual, c);

                        j->nivel->vx_max = 5;
                        j->nivel->vx_min = -5;

                        j->nivel->vy_max = 50;
                        j->nivel->vy_min = 20;

                        j->nivel->palavras_max = 20;
                        j->nivel->palavras_min = 15;

                        j->total = 500;

                        inicializa_palavras(j);
                    }
                }
            }
            break;

        //verifica a digitação das palavras

        case 1:
            char input = tela_tecla();

            //se não tem mais palavras na tela, acaba a jogada
            
            if((j->saiu) == j->n_palavras)
            {
                j->tela_atual = 2;
                break;
            }
            
            //mantém fora do loop enquanto o jogador não apertar uma tecla

            if(input == '\0')
            {
                break;
            }

            //seleciona uma palavra

            if(j->sel == -1)
            {
                for(int i = 0; i < j->n_palavras; i++)
                {
                    palavra_t *p = &(j->palavras[i]);
                    if(input == p->t[0] && p->x >= 0 && p->x <= 500 && p->y >= 0 && p->y <= 500)
                    {
                        j->sel = i;
                        break;
                    }
                }
            }

            //se não tem palavra selecionada, cai fora para testar com outro input

            if(j->sel == -1)
            {
                break;
            }

            palavra_t *p = &(j->palavras[j->sel]);
            for(int k = 0; k < 30; k++)
            {
                //se a letra selecionada já foi digitada, passa para a próxima

                if(p->t[k] == ' ')
                {
                    continue;
                }

                //se tá certo, apaga a letra e pinta de vermelho

                if(input == p->t[k])
                {
                    p->t[k] = ' ';
                    p->cor = vermelho;

                    //se acabou a palavra, da os pontos, conta que saiu e desseleciona a palavra

                    if(p->t[k+1] == '\0')
                    {
                        j->pontos += aleatorio(10, 100);
                        p->invalida = true;
                        j->saiu += 1;
                        j->sel = -1;
                        p->cor = branco;
                    }
                }
                break;
            }
            printf("%d\n", j->saiu);
            break;

        case 2:
            break;
    }
}
