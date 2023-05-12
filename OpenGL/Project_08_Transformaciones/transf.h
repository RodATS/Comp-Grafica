#ifndef TRANSF_H
#define TRANSF_H

#include <iostream>
#include <math.h>
using namespace std;

//-----------------CLASE Transformaciones-------------------------------------------------------------
struct Transformaciones {
    float rot_x[4][4], rot_y[4][4], rot_z[4][4], T[4][4], S[4][4];

    //Solo inicializa las matrices
    Transformaciones() {
        float temp[4][4] = { {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,1} };
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                rot_x[i][j] = temp[i][j];
                rot_y[i][j] = temp[i][j];
                rot_z[i][j] = temp[i][j];
                T[i][j] = temp[i][j];
                S[i][j] = temp[i][j];
            }
        }
    }

	//matriz1 es x y z, el vertice
    float* multiplicacion(float matriz1[3], float matriz2[4][4], bool vector) {
    
        float new_matriz1[4][1];
        for (int i = 0; i < 3; i++) {
            new_matriz1[i][0] = matriz1[i];
        }
		
		//saber si es escala
        if (!vector) {
            new_matriz1[3][0] = 0;
        }
        else {
            new_matriz1[3][0] = 1;
        }
		
		
		for (int i = 0; i < 4; i++) {
			matriz1[i]=0;
			for (int k = 0; k < 4; k++) {
				matriz1[i] += matriz2[i][k] * new_matriz1[k][0];
			}
            
        }

        return matriz1;
    }

    //Rotacion en x
    void Rotacion_x(float angle, float* &vertices, int size) {
		cout<<"Rotamos en x:\n";
        //Convertimos a grados
        float degree = angle * (3.141592653589793238463 / 180);
        float temp[3];
        float* res = new float[3];
        //Construimos la matrix rotacion_x con el angulo 
        rot_x[0][0] = 1;
        rot_x[1][1] = cos(degree); rot_x[1][2] = -sin(degree);
        rot_x[2][1] = sin(degree); rot_x[2][2] = cos(degree);
        //Multiplicamos por todos los vectores recibidos y actualizamos nuestra matriz
        for (int i = 0; i < size; i += 3) {
            temp[0] = vertices[i];
            temp[1] = vertices[i + 1];
            temp[2] = vertices[i + 2];
            res = multiplicacion(temp, rot_x, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			cout<<res[0] << " - " << res[1] << " - " << res[2]<<endl;
        }
		
    }

    void Rotacion_y(float angle, float* &vertices, int size) {
		cout<<"Rotamos en y:\n";
        float degree = angle * (3.141592653589793238463 / 180);
        float temp[3];
        float* res = new float[3];
        rot_y[0][0] = cos(degree); rot_y[0][2] = sin(degree);
        rot_y[1][1] = 1;
        rot_y[2][0] = -sin(degree); rot_y[2][2] = cos(degree);
        for (int i = 0; i < size; i += 3) {
            temp[0] = vertices[i];
            temp[1] = vertices[i + 1];
            temp[2] = vertices[i + 2];
            res = multiplicacion(temp, rot_y, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			cout<<res[0] << " - " << res[1] << " - " << res[2]<<endl;
        }
    }

    void Rotacion_z(float angle, float* &vertices, int size) {
		cout<<"Rotamos en z:\n";
        float degree = angle * (3.141592653589793238463 / 180);
        float temp[3];
        float* res = new float[3];
        rot_z[0][0] = cos(degree); rot_z[0][1] = -sin(degree);
        rot_z[1][0] = sin(degree); rot_z[1][1] = cos(degree);
        rot_z[2][2] = 1;
        for (int i = 0; i < size; i += 3) {
            temp[0] = vertices[i];
            temp[1] = vertices[i + 1];
            temp[2] = vertices[i + 2];
            res = multiplicacion(temp, rot_z, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			cout<<res[0] << " - " << res[1] << " - " << res[2]<<endl;
        }
		
    }

    void Translation(float tx, float ty, float tz, float* &vertices, int size) {
		cout<<"Trasladamos los puntos:\n";
        float temp[3];
        float* res = new float[3];
        T[0][0] = 1; T[0][3] = tx;
        T[1][1] = 1; T[1][3] = ty;
        T[2][2] = 1; T[2][3] = tz;
        for (int i = 0; i < size; i += 3) {
            temp[0] = vertices[i];
            temp[1] = vertices[i + 1];
            temp[2] = vertices[i + 2];
            res = multiplicacion(temp, T, 1);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			cout<<res[0] << " - " << res[1] << " - " << res[2]<<endl;
        }
    }

    void Scale(float sx, float sy, float sz, float* &vertices, int size) {
		cout<<"Escalamos los puntos:\n";
        float temp[3];
        float* res = new float[3];
        S[0][0] = sx;
        S[1][1] = sy;
        S[2][2] = sz;
        for (int i = 0; i < size; i += 3) {
            temp[0] = vertices[i];
            temp[1] = vertices[i + 1];
            temp[2] = vertices[i + 2];
            res = multiplicacion(temp, S, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			cout<<res[0] << " - " << res[1] << " - " << res[2]<<endl;
        }
    }
};



#endif
