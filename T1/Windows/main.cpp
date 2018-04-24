#include "windows.h"
#include "main.h"

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{

    /*
    ===============================
    Variáveis auxiliares
    cor(R,G,B,A)
    ===============================
    */

    int cor0[4] = {255, 0, 0, 255};
    int cor1[4] = {0, 255, 0, 255};
    int cor2[4] = {0, 0, 255, 255};

//LEMBRAR DE DESCOMENTAR A FUNÇÃO DESEJADA E COMENTAR AS OUTRAS DUAS//

    /*
    ===============================
    Função para desenhar píxels
    PutPixel(x,y,cor);
    ===============================
    */

    /*PutPixel(256,106,cor0);
    PutPixel(356,206,cor1);
    PutPixel(156,306,cor2);*/

//LEMBRAR DE DESCOMENTAR A FUNÇÃO DESEJADA E COMENTAR AS OUTRAS DUAS//

    /*
    ===============================
    Função para desenhar linhas
    DrawLine(x0, y0, x1, y1, cor0, cor1)
    ===============================
    */

    //DrawLine(120,60,120,320,cor0,cor1);
    //DrawLine(306, 206, 256, 106,cor2,cor0);

//LEMBRAR DE DESCOMENTAR A FUNÇÃO DESEJADA E COMENTAR AS OUTRAS DUAS//

    /*
    ===============================
    Função para desenhar triângulos
    DrawTriangle(xa, ya, xb, yb, xc, yc, cor0, cor1, cor2);
    ===============================
    */

    DrawTriangle(106, 406, 256, 106, 406, 406, cor0, cor1, cor2);
    DrawTriangle(83, 256, 512, 83, 256, 512, cor1, cor2, cor0);
    
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}

