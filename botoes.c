//Rodrigo Schmidt Becker

#include "tela.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
    int n_palavras;
    float cadencia_max;
    float cadencia_min;
    float vx_max;
    float vx_min;
    float vy_max;
    float vy_min;
} grau_t;

typedef struct
{
    botao_t *botoes;
    float x_max;
    float x_min;
    float y_max;
    float y_min;
    bool fim;
} tela_t;

void abandonar(char *msg)
{
    printf("Erro: %s\n", msg);
    exit(1);
}

int aleatorio(int m, int M)
{
    return rand()%(M-m+1)+m;
}

void inicializar_jogo(tela_t *t)
{
    //botões

    for(int i = 0; i < 3; i++)
    {
        botao_t *b = &(t->botoes[i]);

        b->sup_esq.x = 150;
        b->sup_esq.y = 110 + 100*i;

        b->centro.x = 350;
        b->centro.y = 190 + 100*i;

        b->inf_dir.x = 250;
        b->inf_dir.y = 150 + 100*i;
    }

    //limites de movimento

    t->x_max = 500;
    t->x_min = 0;
    t->y_max = 500;
    t->y_min = 0;

    t->fim = false;
    
}

void desenhar_botao(botao_t b, char t[])
{
    tela_retangulo(b.sup_esq.x, b.sup_esq.y, b.inf_dir.x, b.inf_dir.y, 0, branco, branco);
    tela_texto(b.centro.x, b.centro.y, 30, preto, t);
}

void desenhar_tela(tela_t *t)
{
    desenhar_botao(t->botoes[0], "FÁCIL");
    desenhar_botao(t->botoes[1], "MÉDIO");
    desenhar_botao(t->botoes[2], "DIFÍCIL");
    
}

void mover_palavras(tela_t *t)
{
    
}

void verificar_entradas(tela_t *t)
{
    if (tela_rato_clicado())
    {

        int mouse_x = tela_rato_x_clique();
        int mouse_y = tela_rato_y_clique();

        if (mouse_x >= 150 && mouse_x <= 350)
        {
            if (mouse_y >= 110 && mouse_y <= 190)
            {
            printf("Fácil\n");
            }
            else if (mouse_y >= 210 && mouse_y <= 290)
            {
            printf("Médio\n");
            }
            else if (mouse_y >= 310 && mouse_y <= 390)
            {
            printf("Difícil\n");
            }
        }
    }
}

int main()
{

    tela_t t;
    inicializar_jogo(&t);

    srand(relogio());

    tela_inicio(500, 500, "Mata-palavras");

    while(!t.fim)
    {
        desenhar_tela(&t);
        tela_atualiza();
        mover_palavras(&t);
        verificar_entradas(&t);
    }

    tela_fim();
    return 0;
}
