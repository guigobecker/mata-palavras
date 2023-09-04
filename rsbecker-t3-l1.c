//Rodrigo Schmidt Becker

//backspace pausa o jogo
//se não digitar nada durante o jogo, os contadores bugam

#include "tela.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

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
    //deu um bug inexplicável com as primeiras duas variáveis desse struct, aí criei duas variáveis inúteis
    //pra desbugar as outras, único jeito que consegui pra resolver

    int _;
    int __;

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
    int pontos_p;
    float tpm_p;
    float acerto_p;
} placar_t;

typedef struct
{
    int tela_atual;
    int n_palavras;
    int total;
    int pontos;
    int pontos_total;
    float contador_jogada;
    int vidas;
    int nivel_selecionado;
    int grau;
    int saiu;
    int sel;
    float teclas_total;
    float teclas_certas;
    float taxa;
    float tempo_pausa;

    botao_t *botoes;
    palavra_t *palavras;
    nivel_t *nivel;
    vetor_t *todas;
    placar_t *placar;

    float tpm;
    float minutos;

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

    j->palavras = malloc(14 * sizeof(palavra_t));
    j->nivel = malloc(sizeof(nivel_t));
    j->botoes = malloc(7 * sizeof(botao_t));
    j->todas = malloc(500 * sizeof(vetor_t));
    j->placar = malloc(6 * sizeof(placar_t));

    j->tela_atual = 0;

    //botões de escolha de nível

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

    //botão de continuar e o de menu inicial
    
    int v = 0;

    for(int i = 3; i < 5; i++)
    {
        botao_t *b = &(j->botoes[i]);

        b->sup_esq.x = 150;
        b->sup_esq.y = 210 + 100*v;

        b->centro.x = 250;
        b->centro.y = 250 + 100*v;

        b->inf_dir.x = 350;
        b->inf_dir.y = 290 + 100*v;

        v++;
    }
    
    //botão de voltar

    j->botoes[5].sup_esq.x = 150;
    j->botoes[5].sup_esq.y = 400;

    j->botoes[5].centro.x = 250;
    j->botoes[5].centro.y = 440;

    j->botoes[5].inf_dir.x = 350;
    j->botoes[5].inf_dir.y = 480;
    
    //necessário para que o while na main funcione

    j->fim = false;
}

void move_palavra(jogo_t *j, int i)
{
        palavra_t *p = &(j->palavras[i]);
        p->x += p->vx * SEGUNDOS_POR_QUADRO;
        p->y += p->vy * SEGUNDOS_POR_QUADRO;

        //se entrou na área do jogador, perde vida e apaga a palavra

        if(p->invalida == false && p->x > 225 && p->x < 275 && p->y > 400 && p->y < 450)
        {
            j->vidas -= 1;
            p->invalida = true;

            j->saiu += 1;

            char vazio[] = " ";
            strcpy(p->t, vazio);
        }
    
        //se a palavra saiu da tela, conta que saiu;
        
        if(j->palavras[i].invalida == false && (j->palavras[i].x < 0 || 
           j->palavras[i].x > 500 || j->palavras[i].y > 500))
        {

            j->palavras[i].invalida = true;
            j->saiu += 1;
            //printf("%d\n", j->saiu);

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
    tela_texto(b.centro.x, b.centro.y, 25, preto, t);
}

void desenha_tela(jogo_t *j)
{
        //inicializa as fontes

        ALLEGRO_FONT *fonte = NULL;
        ALLEGRO_FONT *jogada = NULL;
        ALLEGRO_COLOR branca = al_map_rgb_f(1, 1, 1);

        fonte = al_load_font("DejaVuSans.ttf", 15, 0);
        jogada = al_load_font("DejaVuSans.ttf", 25, 0);

    switch(j->tela_atual)
    {
        case 0:

            //desenha os botões

            desenha_botao(j->botoes[0], "FÁCIL");
            desenha_botao(j->botoes[1], "MÉDIO");
            desenha_botao(j->botoes[2], "DIFÍCIL");

            break;

        case 1:

            //palavras

            for(int i = 0; i < j->n_palavras; i++)
            {
                desenha_palavra(j->palavras[i]);
            }

            //área do jogador

            tela_retangulo(225, 400, 275, 450, 0, transparente, verde);

            //pontos e vidas

            al_draw_textf(fonte, branca, 15, 455, ALLEGRO_ALIGN_LEFT, "PONTOS: %d", j->pontos);
            al_draw_textf(fonte, branca, 15, 475, ALLEGRO_ALIGN_LEFT, "VIDAS: %d", j->vidas);

            break;

        case 2:

            //desenha tela de jogada concluída

            al_draw_textf(jogada, branca, 250, 150, ALLEGRO_ALIGN_CENTRE, "JOGADA %.0f CONCLUÍDA", j->contador_jogada);

            desenha_botao(j->botoes[3], "CONTINUAR");

            al_draw_textf(fonte, branca, 15, 435, ALLEGRO_ALIGN_LEFT, "PONTOS: %d", j->pontos);
            al_draw_textf(fonte, branca, 15, 455, ALLEGRO_ALIGN_LEFT, "TPM: %.0f", j->teclas_total / j->minutos);
            al_draw_textf(fonte, branca, 15, 475, ALLEGRO_ALIGN_LEFT, "ACERTO: %.0f%%", (j->teclas_certas * 100) / j->teclas_total);

            break;

        case 3:

            //desenha tela de game over

            al_draw_textf(jogada, branca, 250, 150, ALLEGRO_ALIGN_CENTRE, "GAME OVER");

            desenha_botao(j->botoes[3], "PLACAR");
            desenha_botao(j->botoes[4], "MENU INICIAL");

            al_draw_textf(fonte, branca, 15, 435, ALLEGRO_ALIGN_LEFT, "PONTOS: %d", j->pontos_total);
            al_draw_textf(fonte, branca, 15, 455, ALLEGRO_ALIGN_LEFT, "TPM: %.0f", j->tpm);
            al_draw_textf(fonte, branca, 15, 475, ALLEGRO_ALIGN_LEFT, "ACERTO: %.0f%%", j->taxa);

            break;

        case 4:

            //desenha tela de pausa

            al_draw_textf(jogada, branca, 250, 150, ALLEGRO_ALIGN_CENTRE, "PAUSA");

            desenha_botao(j->botoes[3], "CONTINUAR");

            break;

        case 5:

            //desenha tela de placar

            for(int i = 0; i < 5; i++)
            {
                al_draw_textf(fonte, branca, 250, 10 + 80 * i, ALLEGRO_ALIGN_CENTER, "PONTOS: %d", j->placar[i].pontos_p);
                al_draw_textf(fonte, branca, 250, 30 + 80 * i, ALLEGRO_ALIGN_CENTER, "TPM: %.0f", j->placar[i].tpm_p);
                al_draw_textf(fonte, branca, 250, 50 + 80 * i, ALLEGRO_ALIGN_CENTER, "ACERTO: %.0f%%", j->placar[i].acerto_p);
            }

            desenha_botao(j->botoes[5], "VOLTAR");

            break;
    }

    tela_atualiza();
}

void inicializa_placar(jogo_t *j)
{
    FILE *arq;

    arq = fopen("placar.txt", "r");
    
    if (arq == NULL)
    {
        printf("Não foi possível abrir o arquivo 'placar' para leitura.\n");
    }

    for(int i = 0; i < 5; i++)
    {
        fscanf(arq, "%d%f%f", &j->placar[i].pontos_p, &j->placar[i].tpm_p, &j->placar[i].acerto_p);
    }
    
    fclose(arq);
}

void inicializa_nivel(jogo_t *j)
{
    //inicializa variáveis

    j->contador_jogada = 0;
    j->grau = 0;
    j->pontos_total = 0;
    j->sel = -1;
    j->saiu = 0;
    j->tpm = 0;

    inicializa_placar(j);
    //printf("%d\n%f\n%f\n\n", j->placar[0].pontos_p, j->placar[0].tpm_p, j->placar[0].acerto_p);

    switch(j->nivel_selecionado)
    {
        case 0:

            //número total de palavras no vetor

            j->total = 50;

            //número de vidas do nível

            j->vidas = 5;

            //inicializa os graus do nível

            for(int i = 0; i < 3; i++)
            {
                nivel_t *n = &(j->nivel[i]);

                //define o número de palavras de cada grau

                n->palavras_max = 6 + 2 * i;
                n->palavras_min = 3 + 2 * i;

                //define os limites de velocidade de cada grau

                n->vx_max = 10 - 2 * i;
                n->vx_min = - 10 + 2 * i;

                //printf("%f\n%f\n\n", n->vw_max, n->vw_min);

                n->vy_max = 20 + 5 * i;
                n->vy_min = 15 + 2.5 * i;
            }

            break;

        case 1:

            j->total = 150;

            j->vidas = 3;


            for(int i = 0; i < 3; i++)
            {
                nivel_t *n = &(j->nivel[i]);

                n->palavras_max = 8 + 2 * i;
                n->palavras_min = 4 + 2 * i;

                n->vx_max = 10 - 2 * i;
                n->vx_min = - 10 + 2 * i;

                n->vy_max = 25 + 5 * i;
                n->vy_min = 20 + 2.5 * i;
            }

            break;

        case 2:

            j->total = 500;

            j->vidas = 1;

            for(int i = 0; i < 3; i++)
            {
                nivel_t *n = &(j->nivel[i]);

                n->palavras_max = 10 + 2 * i;
                n->palavras_min = 5 + 2 * i;

                n->vx_max = 10 - 2 * i;
                n->vx_min = - 10 + 2 * i;

                n->vy_max = 30 + 5 * i;
                n->vy_min = 25 + 2.5 * i;
            }

            break;
    }

}

void inicializa_palavras(jogo_t *j)
{
    //nomeia a variável nível atual com o nome do arquivo referente ao nível escolhido

    switch(j->nivel_selecionado)
    {
        case 0:

            char a[] = "p1.txt";
            strcpy(j->nivel->atual, a);
            break;

        case 1:

            char b[] = "p2.txt";
            strcpy(j->nivel->atual, b);
            break;

        case 2:

            char c[] = "p3.txt";
            strcpy(j->nivel->atual, c);
            break;
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
}

void inicializa_jogada(jogo_t *j)
{
    //guarda o tempo em que começou a jogada

    j->minutos = al_get_time();
    //printf("início: %f\n\n", j->minutos);

    //zera a variável que checa quantas palavras saíram da tela

    j->saiu = 0;

    //zera o contador de pontos da jogada

    j->pontos = 0;

    //zera os contadores de teclas

    j->teclas_total = 0;
    j->teclas_certas = 0;

    //define o número de palavras da jogada

    j->n_palavras = aleatorio(j->nivel[j->grau].palavras_min, j->nivel[j->grau].palavras_max);

    for(int i = 0; i < j->n_palavras; i++)
    {
        palavra_t *p = &(j->palavras[i]);

        //inicializa as variáveis

        p->x = aleatorio(200, 300);
        p->y = aleatorio(-280, -10);
        p->tam = aleatorio(15, 25);
        p->cor = branco;
        p->invalida = false;

        //define a velocidade de cada palavra

        p->vx = aleatorio(j->nivel[j->grau].vx_min, j->nivel[j->grau].vx_max);
        p->vy = aleatorio(j->nivel[j->grau].vy_min, j->nivel[j->grau].vy_max);

        //printf("vx min: %f\nvx max: %f\n\n",j->nivel[j->grau].vx_min, j->nivel[j->grau].vx_max);
    }

    //copia as palavras sorteadas para o vetor que será usado na jogada

    for(int w = 0; w < j->n_palavras; w++)
    {
        strcpy(j->palavras[w].t, j->todas[aleatorio(0, j->total)].palavra);
    }

    //atualiza a tela atual

    j->tela_atual = 1;
}

void verifica_entradas(jogo_t *j)
{
    switch(j->tela_atual)
    {
        //verifica se algum botão foi clicado

        case 0:

            //menu inicial

            if (tela_rato_clicado())
            {
                int mouse_x = tela_rato_x_clique();
                int mouse_y = tela_rato_y_clique();

                if (mouse_x >= 150 && mouse_x <= 350)
                {
                    if (mouse_y >= 110 && mouse_y <= 190)
                    {
                        //nível fácil

                        j->nivel_selecionado = 0;

                        //inicializa o nível, as palavras e a jogada

                        inicializa_nivel(j);
                        inicializa_palavras(j);
                        inicializa_jogada(j);
                    }
                    else if (mouse_y >= 210 && mouse_y <= 290)
                    {

                        //nível médio

                        j->nivel_selecionado = 1;

                        inicializa_nivel(j);
                        inicializa_palavras(j);
                        inicializa_jogada(j);
                    }
                    else if (mouse_y >= 310 && mouse_y <= 390)
                    {

                        //nível difícil

                        j->nivel_selecionado = 2;

                        inicializa_nivel(j);
                        inicializa_palavras(j);
                        inicializa_jogada(j);
                    }
                }
            }

            break;

        case 1:

        //verifica a digitação das palavras

            char input = tela_tecla();

            //pausa o jogo

            if(input == '\b')
            {
                //guarda o tempo em que apertou pausa

                j->tempo_pausa = al_get_time();

                j->tela_atual = 4;
            }

            //se não tem mais palavras na tela, acaba a jogada
            
            if((j->saiu) == j->n_palavras)
            {
                j->contador_jogada += 1;
                j->pontos_total += j->pontos;

                //calcula quantos minutos se passaram na jogada

                j->minutos = (al_get_time() - j->minutos - j->tempo_pausa) / 60;

                //guarda o tpm geral

                //printf("%f\n\n", j->tpm);

                j->tpm = (j->teclas_total / j->minutos + j->tpm) / j->contador_jogada;

                //printf("j.tpm = %f\nj.teclas_total = %f\nj.minutos = %f\nj.contador_jogada = %f\n\n",
                //       j->tpm, j->teclas_total, j->minutos, j->contador_jogada);

                //guarda a taxa de acertos

                j->taxa = (((j->teclas_certas * 100.0f) / j->teclas_total) + j->taxa) / j->contador_jogada;

                j->tela_atual = 2;

                break;
            }

            //se perdeu todas as vidas, game over

            if(j->vidas == 0)
            {
                j->contador_jogada += 1;
                j->pontos_total += j->pontos;

                j->minutos = (al_get_time() - j->minutos) / 60;

                j->tpm = (j->teclas_total / j->minutos + j->tpm) / j->contador_jogada;

                j->taxa = (((j->teclas_certas * 100.0f) / j->teclas_total) + j->taxa) / j->contador_jogada;
                
                //ajusta o placar

                if(j->pontos_total > j->placar[4].pontos_p)
                {
                    j->placar[4].pontos_p = j->pontos_total;
                    j->placar[4].tpm_p = j->tpm;
                    j->placar[4].acerto_p = j->taxa;

                    for(int i = 3; i >= 0; i--)
                    {
                        if(j->pontos_total > j->placar[i].pontos_p)
                        {
                            j->placar[5].pontos_p = j->placar[i].pontos_p;
                            j->placar[5].tpm_p = j->placar[i].tpm_p;
                            j->placar[5].acerto_p = j->placar[i].acerto_p;

                            j->placar[i].pontos_p = j->pontos_total;
                            j->placar[i].tpm_p = j->tpm;
                            j->placar[i].acerto_p = j->taxa;

                            j->placar[i+1].pontos_p = j->placar[5].pontos_p;
                            j->placar[i+1].tpm_p = j->placar[5].tpm_p;
                            j->placar[i+1].acerto_p = j->placar[5].acerto_p;
                        }
                    }

                        //atualiza o arquivo com o placar

                        FILE *arq;

                        arq = fopen("placar.txt", "w");

                        if (arq == NULL)
                        {
                            printf("Não foi possível abrir o arquivo 'placar' para gravação.\n");
                        }

                        for(int i = 0; i < 5; i++)
                        {
                            fprintf(arq, "%d\n%f\n%f\n", j->placar[i].pontos_p, j->placar[i].tpm_p, j->placar[i].acerto_p);
                        }

                        fclose(arq);

                }

                j->tela_atual = 3;
            }

            //mantém fora do loop enquanto o jogador não apertar uma tecla

            if(input == '\0')
            {
                break;
            }

            if(input != '\0')
            {
                j->teclas_total += 1;
                //printf("%d\n", j->teclas_total);
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

                //se tá certo, apaga a letra

                if(input == p->t[k])
                {
                    p->t[k] = ' ';
                    p->cor = vermelho;

                    j->pontos += aleatorio(1, 5);

                    j->teclas_certas += 1;

                    //se acabou a palavra, da os pontos, conta que saiu e desseleciona a palavra

                    if(p->t[k+1] == '\0')
                    {
                        p->invalida = true;
                        j->saiu += 1;
                        j->sel = -1;
                        p->cor = branco;
                        //printf("%d\n", j->saiu);
                    }
                }
                break;
            }
            break;

        case 2:

            //jogada concluída

            if (tela_rato_clicado())
            {
                int mouse_x = tela_rato_x_clique();
                int mouse_y = tela_rato_y_clique();

                if (mouse_x >= 150 && mouse_x <= 350)
                {
                    if (mouse_y >= 210 && mouse_y <= 290)
                    {
                        //atualiza  o grau da jogada

                        if(j->grau < 2)
                        {
                            j->grau += 1;
                        }
                        
                        inicializa_jogada(j);
                    }
                }
            }

            break;

        case 3:

            //game over

            if (tela_rato_clicado())
            {
                int mouse_x = tela_rato_x_clique();
                int mouse_y = tela_rato_y_clique();
                
                if (mouse_x >= 150 && mouse_x <= 350)
                {
                    //clicou no placar

                    if (mouse_y >= 210 && mouse_y <= 290)
                    {
                        j->tela_atual = 5;
                        break;
                    }
                
                    //clicou no menu inicial

                    else if (mouse_y >= 310 && mouse_y <= 390)
                    {
                        j->tela_atual = 0;
                    }
                }
            }

            break;

        case 4:

            //pausa

            if (tela_rato_clicado())
            {
                int mouse_x = tela_rato_x_clique();
                int mouse_y = tela_rato_y_clique();
                
                if (mouse_x >= 150 && mouse_x <= 350)
                {
                    //clicou no continuar

                    if (mouse_y >= 210 && mouse_y <= 290)
                    {
                        //guarda o tempo em que ficou pausado

                        j->tempo_pausa = al_get_time() - j->tempo_pausa;

                        //volta pro jogo

                        j->tela_atual = 1;

                        break;
                    }
                }
            }

            break;

        case 5:

            //placar

            if (tela_rato_clicado())
            {
                int mouse_x = tela_rato_x_clique();
                int mouse_y = tela_rato_y_clique();
                
                if (mouse_x >= 150 && mouse_x <= 350)
                {
                    //clicou no voltar

                    if (mouse_y >= 400 && mouse_y <= 480)
                    {
                        j->tela_atual = 3;
                        break;
                    }
                }
            }

            break;
    }
}