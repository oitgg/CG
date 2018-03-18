#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"
#include "math.h"
#include "stdio.h"
#include "cstdlib"

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************
void PutPixel(int x, int y, int rgba[]) {

    //Verifica se x e y são posições válidas para ponto
    if(x < 0 ||
       y < 0)
        return;

    //Desenha cada um dos 4 canais de cor do ponto desejado
    FBptr[4*y*IMAGE_WIDTH + 4*x]      = rgba[0];
    FBptr[4*y*IMAGE_WIDTH + 4*x +1]   = rgba[1];
    FBptr[4*y*IMAGE_WIDTH + 4*x +2]   = rgba[2];
    FBptr[4*y*IMAGE_WIDTH + 4*x +3]   = rgba[3];

}

void DrawLine(int x0, int y0, int x1, int y1, int rgbaP0[], int rgbaP1[]) {

    int dx, dy, x, y, step, pY, pX, p;
    float dTotal, compR, compG, compB, compA, r, g, b, a, iR, iG, iB, iA;

    //Calcula as distâncias entre as posições x em relação ao eixo
    //e entre as posições y em relação ao eixo y
    dx = x1 - x0;
    dy = y1 - y0;

    //Define orientação dos quadrantes de referência e,
    //consequentemente, passo por incremento para x e y.
    dx < 0 ? pX = -1 : pX = 1;
    dy < 0 ? pY = -1 : pY = 1;

    //Transforma os valores das distâncias para módulo destes
    dx = abs(dx);
    dy = abs(dy);

    //Calcula o número de passos necessários aproximados,
    //com base no eixo que mais será incrementado.
    dx > dy ? step = dx : step = dy;
    p = step/2;

    //Calcula a distância média entre ambos os pontos (x0,y0) e (x1,y1),
    //a fim de aproximar o número de pontos na reta. Com o objetivo de
    //usar esse valor na interpolação de cores.
    dTotal = sqrt((dx*dx) + (dy*dy));

    //Compara os valores de RGB para os pontos final e inicial
    compR = rgbaP1[0] - rgbaP0[0];
    compG = rgbaP1[1] - rgbaP0[1];
    compB = rgbaP1[2] - rgbaP0[2];
    compA = rgbaP1[3] - rgbaP0[3];

    //Calcula os incrementos em cima da cor a serem feitos por ponto
    iR = compR / dTotal;
    iG = compG / dTotal;
    iB = compB / dTotal;
    iA = compA / dTotal;

    //Valores iniciais para cor
    r = rgbaP0[0];
    g = rgbaP0[1];
    b = rgbaP0[2];
    a = rgbaP0[3];

    //Inicialização do ponto atual
    x = x0;
    y = y0;

    //Desenha ponto inicial com cor apropriada
    PutPixel(x, y, rgbaP0);

    while(step > 0) {
        //Cálculo das posições x e y do ponto em relação à
        //referência de orientação.
        if(dx > dy) {
            x += pX;
            p += dy;

            //Aproximação e correção do erro em relação ao eixo de orientação
            if(p >= dx) {
                y += pY;
                p -= dx;
            }
        } else {
            y += pY;
            p += dx;

            //Aproximação e correção do erro em relação ao eixo de orientação
            if(p >= dy) {
                x += pX;
                p -= dy;
            }
        }

        //Interpolação/atualização do valor da cor para o ponto atual
        r += iR;
        g += iG;
        b += iB;
        a += iA;

        int auxRGBA[4] = {r, g, b, a};

        //Desenha ponto atual com cor calculada
        PutPixel(x, y, auxRGBA);

        //Atualização da variável de iteração do laço
        step--;
    }

    //Desenha ponto final com cor apropriada
    PutPixel(x, y, rgbaP1);
}

void DrawTriangle(int xa, int ya, int xb, int yb, int xc, int yc,
                  int rgbaP0[], int rgbaP1[], int rgbaP2[]) {

    DrawLine(xa, ya, xb, yb, rgbaP0, rgbaP1);
    DrawLine(xb, yb, xc, yc, rgbaP1, rgbaP2);
    DrawLine(xc, yc, xa, ya, rgbaP2, rgbaP0);

}

#endif // _MYGL_H_

