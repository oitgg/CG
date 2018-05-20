#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"
#include <cmath>
#include <eigen3/Eigen/Dense>

#include "objLoader.h"

#define VERMELHO 255,0,0,255
#define VERDE 0,255,0,255
#define AZUL 0,0,255,255
#define BRANCO 255,255,255,255


typedef Eigen::Matrix<float, 4, 4> Matrix4f; // Matriz quadrada 4x4
typedef Eigen::Matrix<float, 3, 3> Matrix3f; // Matriz quadrada 3x3
typedef Eigen::Matrix<float, 4, 1> Vector4f; // Vetor coluna de 4 elementos
typedef Eigen::Matrix<float, 3, 1> Vector3f; // Vetor coluna de 3 elementos


struct cart{
	unsigned int x, y;
};


// --


class Pixel{
	friend class Line;
private:
	unsigned char _R, _G, _B, _A;
	struct cart _pos;
	int FindPixel(void);
public:
	Pixel(void);
	Pixel(unsigned char R,
		  unsigned char G,
		  unsigned char B,
		  unsigned char A,
		  unsigned int X,
		  unsigned int Y);
	void PutPixel(void);
};

Pixel::Pixel(void){
	this->_R = 0;
	this->_G = 0;
	this->_B = 0;
	this->_A = 0;
	this->_pos.x = 0;
	this->_pos.y = 0;
}

Pixel::Pixel(unsigned char R, unsigned char G, unsigned char B, unsigned char A, unsigned int X, unsigned int Y){
	this->_R = R;
	this->_G = G;
	this->_B = B;
	this->_A = A;
	this->_pos.x = X;
	this->_pos.y = Y;
}

int Pixel::FindPixel(void){
	return this->_pos.x*4 + this->_pos.y*IMAGE_WIDTH*4;
}

void Pixel::PutPixel(void){
	int MemPos = this->FindPixel();
	FBptr[MemPos] = this->_R;
	FBptr[MemPos + 1] = this->_G;
	FBptr[MemPos + 2] = this->_B;
	FBptr[MemPos + 3] = this->_A;
}


// --


class Line{
private:
	Pixel *_p0, *_p1;
public:
	Line(Pixel *start, Pixel *end);
	~Line(void);
	void DrawLine(void);
	Pixel Interpolate(unsigned int x, unsigned int y);
};

Line::Line(Pixel *start, Pixel *end){

	/****
	 *
	 * Para evitar que o ponto inicial dado venha depois do ponto final dado no eixo X, é feita uma verificação.
	 * Caso o ponto x do primeiro ponto dado for depois do segundo ponto dado,
	 * este é colocado como o ponto inicial e aquele como o final.
	 * Assim, o DeltaX da reta nunca será negativo, não sendo necessário considerar casos nos octantes 3, 4, 5 e 6.
	 *
	 ****/

	if(start->_pos.x <= end->_pos.x){
		this->_p0 = start;
		this->_p1 = end;
	}else{
		this->_p0 = end;
		this->_p1 = start;
	}
}

Line::~Line(void){
	delete this->_p0;
	delete this->_p1;
}

Pixel Line::Interpolate(unsigned int x, unsigned int y){
	Pixel output;
	output._pos.x = x;
	output._pos.y = y;

	int deltaX = this->_p1->_pos.x - this->_p0->_pos.x;
	int deltaY = this->_p1->_pos.y - this->_p0->_pos.y;

	float ColorDiff[4];
	float Increment[4];

	ColorDiff[0] = this->_p1->_R - this->_p0->_R;
	ColorDiff[1] = this->_p1->_G - this->_p0->_G;
	ColorDiff[2] = this->_p1->_B - this->_p0->_B;
	ColorDiff[3] = this->_p1->_A - this->_p0->_A;

	int var;

	if(deltaX == 0){
		var = y - this->_p0->_pos.y;
		for(int i = 0; i < 4; ++i)
			Increment[i] = ColorDiff[i]/deltaY;

	}else if(abs(deltaX) == abs(deltaY)){
		var = x - this->_p0->_pos.x;
		for(int i = 0; i < 4; ++i)
			Increment[i] = ColorDiff[i]/deltaX;

	}else if(abs(deltaX) > abs(deltaY)){
		var = x - this->_p0->_pos.x;
		for(int i = 0; i < 4; ++i)
			Increment[i] = ColorDiff[i]/deltaX;

	}else if(abs(deltaX) < abs(deltaY)){
		var = y - this->_p0->_pos.y;
		for(int i = 0; i < 4; ++i)
			Increment[i] = ColorDiff[i]/deltaY;
	}

	output._R = this->_p0->_R + Increment[0]*var;
	output._G = this->_p0->_G + Increment[1]*var;
	output._B = this->_p0->_B + Increment[2]*var;
	output._A = this->_p0->_A + Increment[3]*var;

	return output;
}

void Line::DrawLine(void){
	int deltaX = this->_p1->_pos.x - this->_p0->_pos.x;
	int	deltaY = this->_p1->_pos.y - this->_p0->_pos.y;
	float error;
	Pixel PxBuffer;

	this->_p0->PutPixel();
	unsigned int y_aux, x_aux;


	if(deltaX == 0){
		x_aux = this->_p0->_pos.x;

		if(deltaY > 0){
			for(y_aux = this->_p0->_pos.y; y_aux < this->_p1->_pos.y; y_aux++){
				PxBuffer = this->Interpolate(x_aux, y_aux);
				PxBuffer.PutPixel();
			}
		}else if(deltaY < 0){
			for(y_aux = this->_p0->_pos.y; y_aux > this->_p1->_pos.y; y_aux--){
				PxBuffer = this->Interpolate(x_aux, y_aux);
				PxBuffer.PutPixel();
			}
		}


	}else if(deltaY == 0){ // f(x) = 0
		y_aux = this->_p0->_pos.y;
		for(x_aux = this->_p0->_pos.x; x_aux < this->_p1->_pos.x; x_aux++){
			PxBuffer = this->Interpolate(x_aux, y_aux);
			PxBuffer.PutPixel();
		}


	}else if(deltaX == deltaY){ // f(x) = x
		y_aux = this->_p0->_pos.y;
		for(x_aux = this->_p0->_pos.x + 1; x_aux < this->_p1->_pos.x; x_aux++){
			++y_aux;

			PxBuffer = this->Interpolate(x_aux, y_aux);
			PxBuffer.PutPixel();
		}


	}else if(deltaX == -deltaY){ // f(x) = -x
		y_aux = this->_p0->_pos.y;
		for(x_aux = this->_p0->_pos.x + 1; x_aux < this->_p1->_pos.x; x_aux++){
			--y_aux;

			PxBuffer = this->Interpolate(x_aux, y_aux);
			PxBuffer.PutPixel();
		}


	}else if((abs(deltaX) > abs(deltaY)) && (deltaY > 0)){ // 1º Octante
		error = 2*deltaY - deltaX;
		y_aux = this->_p0->_pos.y + 1/2;

		for(unsigned int x_aux = this->_p0->_pos.x + 1; x_aux < this->_p1->_pos.x; x_aux++){
			if(error <= 0){
				error += deltaY;
			}else{
				error += (deltaY - deltaX);
				++y_aux;
			}

			PxBuffer = this->Interpolate(x_aux, y_aux);
			PxBuffer.PutPixel();
		}


	}else if((abs(deltaX) > abs(deltaY)) && (deltaY < 0)){ // 8º Octante

		error = 2*deltaY + deltaX;
		y_aux = this->_p0->_pos.y - 1/2;

		for(unsigned int x_aux = this->_p0->_pos.x + 1; x_aux < this->_p1->_pos.x; x_aux++){
			if(error >= 0){
				error += deltaY;
			}else{
				error += (deltaY + deltaX);
				--y_aux;
			}
			PxBuffer = this->Interpolate(x_aux, y_aux);
			PxBuffer.PutPixel();
		}


	}else if((abs(deltaX) < abs(deltaY)) && (deltaY > 0)){ // 2º Octante

		error = 2*deltaX - deltaY;
		x_aux = this->_p0->_pos.x + 1/2;

		for(unsigned int y_aux = this->_p0->_pos.y + 1; y_aux < this->_p1->_pos.y; y_aux++){
			if(error <= 0){
				error += deltaX;
			}else{
				error += (deltaX - deltaY);
				++x_aux;
			}
			PxBuffer = this->Interpolate(x_aux, y_aux);
			PxBuffer.PutPixel();
		}


	}else if((abs(deltaX) < abs(deltaY)) && (deltaY < 0)){ // 7º Octante

		error = 2*deltaX + deltaY;
		x_aux = this->_p0->_pos.x + 1/2;

		for(unsigned int y_aux = this->_p0->_pos.y - 1; y_aux > this->_p1->_pos.y; y_aux--){
			if(error <= 0){
				error += deltaX;
			}else{
				error += (deltaX + deltaY);
				++x_aux;
			}
			PxBuffer = this->Interpolate(x_aux, y_aux);
			PxBuffer.PutPixel();
		}


	}

	this->_p1->PutPixel();
}


// --


class Triangle{
private:
	Pixel *_p1, *_p2, *_p3;
	Line *_l1, *_l2, *_l3;
public:
	Triangle(Pixel *px1, Pixel *px2, Pixel *px3);
	void CalcLines(void);
	void DrawTriangle(void);
};

Triangle::Triangle(Pixel *px1, Pixel *px2, Pixel *px3){
	this->_p1 = px1;
	this->_p2 = px2;
	this->_p3 = px3;

	this->_l1 = new Line(this->_p1,this->_p2);
	this->_l2 = new Line(this->_p1,this->_p3);
	this->_l3 = new Line(this->_p2,this->_p3);
}

void Triangle::CalcLines(void){
	this->_l1 = new Line(this->_p1,this->_p2);
	this->_l2 = new Line(this->_p1,this->_p3);
	this->_l3 = new Line(this->_p2,this->_p3);
}

void Triangle::DrawTriangle(void){
	this->_l1->DrawLine();
	this->_l2->DrawLine();
	this->_l3->DrawLine();
}

#endif // _MYGL_H_

