# Comp-Grafica
Los comandos para compilar lo rpogramas del openGL

1. Abrir editores/el visual.
2. Open folder glfw
3. Compilar terminal, no powershell

Con nodepat
1. Empezamos
2. editor/npp_8/notepad++
3. Escoger/lab_compiler
4. Click derecho/DOS prompt (se abre terminal)
5. correr SET_PATH_.bat -> para que reconozca el Cmake y glfw, ya hay g++
6. abiri glfw-amseter
7. correr generate
8. dir se crea el own project
9. cd Own Porject/project_00
10. correr project_00_glfw.exe (RGB)

Si queremos cambiar el main.cpp
1. glfwmaster/ GENERATE
2. ownProject/projectx


Nuevo projecto
1. ownproject
2. copiar el primero y pegar, cambiar nombre a la copia
3. en el CmakeLists, agregar subdeirectory(poryecto nuevo)
4. terminal__
5. Generate
6. agregar o cambiar esto al encabezado
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 
