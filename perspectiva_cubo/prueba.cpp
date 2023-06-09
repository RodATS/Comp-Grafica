#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class Vertices
{
public:
  float x,y,z;
  Vertices(float ini_x, float ini_y, float ini_z){
    x= ini_x; y= ini_y; z= ini_z;
  }

  void Print(){
    cout<<x<<" - "<<y<<" - "<<z<<endl;
  }
};

void move(char ingresa,float &c_x, float &c_y, float &c_z){
  switch (ingresa) 
    {
      case 'Q': 
        c_x++; 
        break;
      
      case 'A': 
        c_x--; 
        break;

      case 'W': 
        c_y++; 
        break;
      
      case 'S': 
        c_y--; 
        break;

      case 'E': 
        c_z++; 
        break;
      
      case 'D': 
        c_z--; 
        break;

      default:
        break;
    }
}

void transformacion(float &x, float &y, float &z, float c_x, float c_y, float c_z){
  x = c_x - x;
  y = c_y - y;
  z = c_z - z;
  x = x / z;
  y = y /z;
  z = z/z;
}

int puntos(float valor, float ref[], int tam)
{
  tam = tam/13;
  for(int i=0;i<13;i++){
    if(valor==ref[i]){
      return i*tam;
    }
  }
}



int main() {
  //2 3 5
  float c_x, c_y, c_z;
  cout<<"x  y  z"<<endl;
  cin>>c_x>>c_y>>c_z;
  cout<<"\n\n";
  
  Vertices A(1,1,1); Vertices B(-1,1,1); Vertices C(1,-1,1); Vertices D(-1,-1,1);
  Vertices E(1,1,-1); Vertices F(-1,1,-1); Vertices G(1,-1,-1); Vertices H(-1,-1,-1);

  Vertices A_O(1,1,1); Vertices B_O(-1,1,1); Vertices C_O(1,-1,1); Vertices D_O(-1,-1,1);
  Vertices E_O(1,1,-1); Vertices F_O(-1,1,-1); Vertices G_O(1,-1,-1); Vertices H_O(-1,-1,-1);
  
  char movimiento;

 char pintado_horizontal = '-', pintado_vertical = '|', pintado_diagonal = '/';
  
  float p2 = 1.0/12.0, p3 = 1.0/6.0, p4 = 1.0/4.0, p5 = 1.0/3.0, p6 = 5.0/12.0, p7= 1.0/2.0, p8 = 7.0/12.0, 
  p9 = 2.0/3.0, p10 = 3.0/4.0, p11 = 5.0/6.0, p12 = 11.0/12.0;
  float pos[13] = {0,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12, 1};


  //Crear la matriz
  int filas = 13, 
  columnas = 13;

  string mat[filas];
  for(int i=0;i<filas;i++)
    mat[i] = *new string[columnas];

  //para la diagonal
  float sA_E, sG_C, sF_B, sH_D;

  do{
    cout<<"x: "<<c_x<<" y: "<<c_y<<" z: "<<c_z<<endl;
    A.x = A_O.x; A.y = A_O.y; A.z = A_O.z;
    B.x = B_O.x; B.y = B_O.y; B.z = B_O.z;
    C.x = C_O.x; C.y = C_O.y; C.z = C_O.z;
    D.x = D_O.x; D.y = D_O.y; D.z = D_O.z;
    E.x = E_O.x; E.y = E_O.y; E.z = E_O.z;
    F.x = F_O.x; F.y = F_O.y; F.z = F_O.z;
    G.x = G_O.x; G.y = G_O.y; G.z = G_O.z;
    H.x = H_O.x; H.y = H_O.y; H.z = H_O.z;
  //Aristas: AB, AC, AE, CD, EF, GH, BD, EG, FH, CG, BF, DH 
    
    transformacion(A.x, A.y, A.z, c_x,  c_y,  c_z);
    transformacion(B.x, B.y, B.z, c_x,  c_y,  c_z);
    transformacion(C.x, C.y, C.z, c_x,  c_y,  c_z);
    transformacion(D.x, D.y, D.z, c_x,  c_y,  c_z);
    transformacion(E.x, E.y, E.z, c_x,  c_y,  c_z);
    transformacion(F.x, F.y, F.z, c_x,  c_y,  c_z);
    transformacion(G.x, G.y, G.z, c_x,  c_y,  c_z);
    transformacion(H.x, H.y, H.z, c_x,  c_y,  c_z);
  
   
  //PINTAR MATRIZ
    for(int i=0; i<filas;i++){
      for(int j=0;j<columnas;j++)
        mat[i][j] = ' ';
    }
  
    
  
    //Aristas: AB, AC, AE, CD, EF, GH, BD, EG, FH, CG, BF, DH 
    //Pintado primer cuadrado ABCD
    int verC_y = filas-1-puntos(C.y,pos,filas), verC_x = puntos(C.x,pos,filas);
    int verD_y = filas-1-puntos(D.y,pos,filas), verD_x = puntos(D.x,pos,filas);
    int verA_y = filas-1-puntos(A.y,pos,filas), verA_x = puntos(A.x,pos,filas);
    int verB_y = filas-1-puntos(B.y,pos,filas), verB_x = puntos(B.x,pos,filas);
  /*
    for(int i= verC_y; i <= verA_y;i++){
      for(int j = verC_x; j<= verB_x; j++){
        mat[i][j] = pintado;
      }
    }*/
    for(int i=verC_x; i<verD_x;i++){
      mat[verC_y][i] = pintado_horizontal;
      mat[verA_y][i] = pintado_horizontal;
    }
  
    for(int i=verC_y; i<verA_y;i++){
      mat[i][verC_x] = pintado_vertical;
      mat[i][verD_x] = pintado_vertical;
    }
  
    //Pintado segunda cuadrado EFGH
    int verG_y = filas-1-puntos(G.y,pos,filas), verG_x = puntos(G.x,pos,filas);
    int verH_y = filas-1-puntos(H.y,pos,filas), verH_x = puntos(H.x,pos,filas);
    int verE_y = filas-1-puntos(E.y,pos,filas), verE_x = puntos(E.x,pos,filas);
    int verF_y = filas-1-puntos(F.y,pos,filas), verF_x = puntos(F.x,pos,filas);
  /*
    for(int i= verG_y; i <= verE_y;i++){
      for(int j = verG_x; j<= verF_x; j++){
        mat[i][j] = pintado;
      }
    }*/
  
    for(int i=verG_x; i<verH_x;i++){
      mat[verG_y][i] = pintado_horizontal;
      mat[verE_y][i] = pintado_horizontal;
    }
  
    for(int i=verG_y; i<verE_y;i++){
      mat[i][verG_x] = pintado_vertical;
      mat[i][verH_x] = pintado_vertical;
    }

    //Diagonales sA_E, sG_C, sF_B, sH_D;
    int v;
    sA_E= (abs(verA_x-verE_x)/abs(verA_y-verE_y));
    if(verA_x > verE_x){
      if(verA_y < verE_y){
        v=verE_y;
        for(int u = verE_x; u < verA_x; u++){
          v -= sA_E;
          mat[v][u] = pintado_diagonal;
        }
      }
      else{
        v=verE_y;
        for(int u = verE_x; u < verA_x; u++){
          v += sA_E;
          mat[v][u] = pintado_diagonal;
        }
      }
    }
    else{
      if(verA_y < verE_y){
        v=verE_y;
        for(int u = verA_x; u < verE_x; u++){
          v -= sA_E;
          mat[v][u] = pintado_diagonal;
        }
      }
      else{
        v=verE_y;
        for(int u = verA_x; u < verE_x; u++){
          v += sA_E;
          mat[v][u] = pintado_diagonal;
        }
      }
    }

    sG_C= (abs(verG_x-verC_x)/abs(verG_y-verC_y));
    if(verG_x > verC_x){
      if(verG_y < verC_y){
        v=verC_y;
        for(int u = verC_x; u < verG_x; u++){
          v -= sG_C;
          mat[v][u] = pintado_diagonal;
        }
      }
      else{
        v=verC_y;
        for(int u = verC_x; u < verG_x; u++){
          v += sG_C;
          mat[v][u] = pintado_diagonal;
        }
      }
    }
    else{
      if(verG_y < verC_y){
        v=verG_y;
        for(int u = verC_x; u < verG_x; u++){
          v -= sG_C;
          mat[v][u] = pintado_diagonal;
        }
      }
      else{
        v=verG_y;
        for(int u = verC_x; u < verG_x; u++){
          v += sG_C;
          mat[v][u] = pintado_diagonal;
        }
      }
    }

    sF_B= (abs(verF_x-verB_x)/abs(verF_y-verB_y));
    if(verF_x > verB_x){
      if(verF_y < verB_y){
        v=verB_y;
        for(int u = verB_x; u < verF_x; u++){
          v -= sF_B;
          mat[v][u] = pintado_diagonal;
        }
      }
      else{
        v=verB_y;
        for(int u = verB_x; u < verF_x; u++){
          v += sF_B;
          mat[v][u] = pintado_diagonal;
        }
      }
    }
    else{
      if(verF_y < verB_y){
        v=verB_y;
        for(int u = verF_x; u < verB_x; u++){
          v -= sF_B;
          mat[v][u] = pintado_diagonal;
        }
      }
      else{
        v=verB_y;
        for(int u = verF_x; u < verB_x; u++){
          v += sF_B;
          mat[v][u] = pintado_diagonal;
        }
      }
    }

    sH_D= (abs(verH_x-verD_x)/abs(verH_y-verD_y));
    if(verH_x > verD_x){
      if(verH_y < verD_y){
        v=verD_y;
        for(int u = verD_x; u < verH_x; u++){
          v -= sH_D;
          mat[v][u] = pintado_diagonal;
        }
      }
      else{
        v=verD_y;
        for(int u = verD_x; u < verH_x; u++){
          v += sH_D;
          mat[v][u] = pintado_diagonal;
        }
      }
    }
    else{
      if(verH_y < verD_y){
        v=verH_y;
        for(int u = verD_x; u < verH_x; u++){
          v -= sH_D;
          mat[v][u] = pintado_diagonal;
        }
      }
      else{
        v=verG_y;
        for(int u = verD_x; u < verH_x; u++){
          v += sH_D;
          mat[v][u] = pintado_diagonal;
        }
      }
    }
  
    //Pintar los vertices
    
    mat[filas-1-puntos(A.y,pos,filas)][puntos(A.x,pos,filas)] = 'A';
    mat[filas-1-puntos(B.y,pos,filas)][puntos(B.x,pos,filas)] = 'B';
    mat[filas-1-puntos(C.y,pos,filas)][puntos(C.x,pos,filas)] = 'C';
    mat[filas-1-puntos(D.y,pos,filas)][puntos(D.x,pos,filas)] = 'D';
    mat[filas-1-puntos(E.y,pos,filas)][puntos(E.x,pos,filas)] = 'E';
    mat[filas-1-puntos(F.y,pos,filas)][puntos(F.x,pos,filas)] = 'F';
    mat[filas-1-puntos(G.y,pos,filas)][puntos(G.x,pos,filas)] = 'G';
    mat[filas-1-puntos(H.y,pos,filas)][puntos(H.x,pos,filas)] = 'H';
  
  
    
    //Imprimir
    for(int i=0; i<filas;i++){
      for(int j=0;j<columnas;j++)
        cout<<mat[i][j];
      cout<<endl;
    }

    cout<<"eje x:Q A, eje y:W S, eje z:E D cerrar:F\n";
    cin>>movimiento;
    move(movimiento,c_x, c_y, c_z);
  }
  while(movimiento != 'F');
  
}
