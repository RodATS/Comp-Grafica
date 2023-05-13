#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "transf.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings (size de la ventana)
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//VERTEX SHADER
//recibe vectores de size 3 (aPos) y devuelve vectores de size 4 (gl_Position)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//FRAGMENT SHADER
//No tiene input, devuelve un vector de size 4 (FragColor)
//Tiene una variable uniform (puede ser modificada mientras el programa corre)
//El output es FragColor que es igual a la variable uniform
const char* fragmentShaderSource = "#version 330 core\n"
"layout(location=0) out vec4 FragColor;\n"
"uniform vec4 u_color;"
"void main()\n"
"{\n"
"   FragColor=u_color;\n"
"}\n\0";

//para registrar key inputs
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

//para los inputs
char figure = 'a';

struct Figure {
    float *vertices;
    unsigned int *indicesTriangulos;
    unsigned int* indicesContorno;
    int vSize, i1Size, i2Size;
    unsigned int VBO, EBO1, EBO2;
    Transformaciones Transform;

    Figure():vertices(NULL),indicesTriangulos(NULL),indicesContorno(NULL),vSize(0),i1Size(0),i2Size(0),VBO(0),EBO1(0),EBO2(0)
    {}

    Figure(unsigned int VBO, unsigned int EBO1, unsigned int EBO2) :vertices(NULL), indicesTriangulos(NULL), indicesContorno(NULL), vSize(0), i1Size(0), i2Size(0)
    {
        this->VBO = VBO;
        this->EBO1 = EBO1;
        this->EBO2 = EBO2;
    }

    void setVertices(int size, float* vertices) {
        vSize = size;
        this->vertices = new float[vSize];
        this->vertices = vertices;
        this->VBO = VBO;
    }

    void setContorno(int size, unsigned int* indices) {
        i2Size = size;
        this->indicesContorno = new unsigned int[i2Size];
        this->indicesContorno = indices;
        this->EBO1 = EBO1;
    }

    void setTriangulos(int size, unsigned int* indices) {
        i1Size = size;
        this->indicesTriangulos = new unsigned int[i1Size];
        this->indicesTriangulos = indices;
        this->EBO2 = EBO2;
    }

    //Crea la figura en el VBO
    void createVer_VBO() {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(float), vertices, GL_STATIC_DRAW);
    }

    //Crea los indices en EBO
    void createCon_EBO2() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, i2Size* sizeof(int), indicesContorno, GL_STATIC_DRAW);
    }

    //Crea los indices del trianguo en EBO2
    void createTri_EBO1() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, i1Size* sizeof(int), indicesTriangulos, GL_STATIC_DRAW);
    }

    void update_buffers() {
        createVer_VBO();
        createCon_EBO2();
        createTri_EBO1();
    }

    void rot_x(float angle) { Transform.Rotacion_x(angle, vertices, vSize); }
    void rot_y(float angle) { Transform.Rotacion_y(angle, vertices, vSize); }
    void rot_z(float angle) { Transform.Rotacion_z(angle, vertices, vSize); }
    void traslation(float tx, float ty, float tz) { Transform.Translation(tx, ty, tz, vertices, vSize); }
    void scale(float sx, float sy, float sz) { Transform.Scale(sx, sy, sz, vertices, vSize); }


};
 //------------------------------------------------------------------------------------------
 //funciones
//para crear el circulo
    int crear_circulo(int angle, Figure &circulo, float *&vertices_circulo2);

//Crea cualquier figura
    void create_figure(Figure& temp, int location);
	
//Para rotar sin girar usando tralsacion
void rotarSinGirar(int &angle, float &ejeX, float &ejeY, bool &crecer);
void agrandarFigura(float &ejeX, float &ejeY,bool &crecer, int &vecesCrecer);


//------------------------------------
int main()
{
    // glfw: initialize and configure (da la version y tipo de perfil en este caso CORE)
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Creamos los shaders usando los source que estan al principio del programa
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader(igual que el vertex shader)
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    //Metemos los shaders en un programa determinado
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //Eliminamos porque ya fueron insertados en el programa y ya no los necesitamos
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //--------------------------------------------------------------------
	//Estrella
    float vertices_estrella[] = {
         0.0f,  0.8f, 0.0f,  //top
         -0.15f, 0.3f, 0.0f,  //bottom left
        0.15f, 0.3f, 0.0f, //bottom right

        -0.6f,  0.3f, 0.0f,  
         0.6f, 0.3f, 0.0f,  
        0.0f, -0.3f, 0.0f,

         -0.2f, -0.1f, 0.0f,
			-0.4f, -0.6f, 0.0f,  

		0.2f, -0.1f, 0.0f,  
		0.4f, -0.6f, 0.0f, 
    };

	
    unsigned int indices_estrella[] = {  
        0, 1, 2,  // first Triangle
        3, 4, 5,   // second Triangle
		6, 7, 5,
		5, 8, 9
    };

    unsigned int indices2_estrella[] = {  
        0,1,3,6,7,5,9,8,4,2,0
    };
    //--------------------------------------------------------------------

    //----------------------------------------------------------------------
	//Flecha
    float vertices_flecha[] = {
        0.0f,  0.5f, 0.0f,  //top 
         -0.4f, 0.0f, 0.0f,  //bottom left
        0.4f, 0.0f, 0.0f, 	//bottom right
		
        -0.25f,  0.0f, 0.0f,  
         -0.25f, -0.5f, 0.0f,  
        0.25f, -0.5f, 0.0f, 
		
         0.25f, 0.0f, 0.0f, 
    };

    unsigned int indices_flecha[] = {
        0, 1, 2,  // first Triangle
        3, 4, 5,   // second Triangle
		3, 6, 5
    };

    unsigned int indices2_flecha[] = {
        0, 1, 3,  // first Triangle
        4, 5, 6,   // second Triangle
        2, 0
    };
    //----------------------------------------------------------------------

    //Creamos los buffers
    //1VAO para guardar los objetos e indices que tengamos
    //3VBO porque tenemos 3 figuras
    //6EBO porque usamos 6 vectores de indices
	//VBO[0] = Estrella
    //VBO[1] = Flecha
    //VBO[2] = Circulo
    unsigned int VAO, VBO[3], EBO[6];
    //Generamos buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(6, EBO);
    
    //Bindeamos el VAO (significa que ahora OPENGL va a utilizar este VAO en especifico)
    glBindVertexArray(VAO);
    
 
    
    Figure Estrella(VBO[0], EBO[0], EBO[1]);
    //Creando vertices
    Estrella.setVertices(sizeof(vertices_estrella) / sizeof(float), vertices_estrella);
    //Indices de los triangulos
    Estrella.setTriangulos(sizeof(indices_estrella) / sizeof(unsigned int), indices_estrella);
    //contorno
    Estrella.setContorno(sizeof(indices2_estrella) / sizeof(unsigned int), indices2_estrella);


    Figure Flecha(VBO[1], EBO[2], EBO[3]);
    Flecha.setVertices(sizeof(vertices_flecha) / sizeof(float), vertices_flecha);
    Flecha.setTriangulos(sizeof(indices_flecha) / sizeof(unsigned int), indices_flecha);
	
    Flecha.setContorno(sizeof(indices2_flecha) / sizeof(unsigned int), indices2_flecha);

    Figure Circulo(VBO[2], EBO[4], EBO[5]);

    //Creando en los buffers
    Estrella.update_buffers();
    Flecha.update_buffers();

    //Ubicacion del uniform
    int location = glGetUniformLocation(shaderProgram, "u_color");
    if (location == -1) {
        printf("Error al conseguir uniform");
        exit(1);
    }
	

    //aqui desbindeamos todo (es buena practica)
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Revisa el input que le demos
    glfwSetKeyCallback(window, keyCallback);

    //crea el circulo 
	float *verRotar;
    int numberOfMovements = crear_circulo(30, Circulo, verRotar);
    float currColor = 0.1f;
    float adder = 0.005f;
	
	
	
	
	Circulo.scale(0.7,0.7,0);
	
	Estrella.scale(0.3,0.3,0);
	Estrella.traslation(-0.2,-0.4,0);
	int angulo = 5;
	float moverX=0.4, moverY = 0.0;
	
	bool agrandar = true;
	float escalarEnX = 0.6, escalarEnY = 0.6;
	//Estrella.traslation(moverX, moverY, 0);
	
	int flag = -1;
	cout<<"----------------"<<endl;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

		//fondo
        glClearColor(0.8f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Le decimos que programa (shaders) queremos usar
        glUseProgram(shaderProgram);
        //Bindeamos nuestro VAO para usar los vertices e indices que acabamos de guardar
        //Un programa puede tener varios VAO por eso especificamos aqui
        glBindVertexArray(VAO);

        if (currColor < 0.0f || currColor>1.0f) {
            adder *= -1.0f;
        }
        currColor += adder;

        //Cambia los colores de las figuras 
        glUniform4f(location, 0.0f, 0.0f, 0.0f, 1.0f);

        //solo cambia el size de las lineas
        glLineWidth((GLfloat)5);

        //Solo cambia el size de los puntos
        glPointSize((GLfloat)5);
		
        switch (figure)
        {
        //Explicando que hace la (estrella), los demas son muy similares
        case 'a': {
           // Estrella.rot_z(0.4);
		   
			//Estrella.rot_z(0.4);
			//Estrella.scale(0,0,0);
			create_figure(Estrella, location);
			
			int time = ((int)glfwGetTime())%numberOfMovements;
			if(flag != time){
					flag = time;
					Estrella.traslation(verRotar[time*2], verRotar[(time*2)+1],0);
					cout<<verRotar[time*2]<<" - "<<verRotar[(time*2)+1]<<endl;
					//cout<<verRotar[0]<<" - "<<verRotar[1]<<endl;
					
			}
			
			//cout<<verRotar[0]<<" - "<<verRotar[1]<<endl;
			//rotarSinGirar(angulo, moverX, moverY, agrandar);
			//Estrella.traslation(0, 0, 0);
			//rotarSinGirar(angulo, moverX, moverY);
			//Estrella.traslation(0.001, 0.001, 0.0);
			//angulo+=5;
			//agrandarFigura(escalarEnX, escalarEnY, agrandar);
			//Estrella.scale(0.3, 0.3, 0);
            Estrella.update_buffers();
			
			glUniform4f(location, 0.1f, 0.7f, 0.2f, 1.0f);
			create_figure(Circulo, location);
			Circulo.rot_y(0.2);
			Circulo.update_buffers();
        }
            break;
        //caso de flecha
        case 'b': {
            create_figure(Flecha, location);
            Flecha.rot_x(0.5);
            Flecha.update_buffers();
        }   
            break;
        //caso del circulo
        case 'c': {
            create_figure(Circulo, location);
			//Circulo.traslation(0.01, 0.01, 0);
        }
            break;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(3, VBO);
    glDeleteBuffers(6, EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

//Dibuja la figura que queramos
void create_figure(Figure &temp, int location) {
    glBindBuffer(GL_ARRAY_BUFFER, temp.VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.EBO1);
    glDrawElements(GL_TRIANGLES, temp.i1Size, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.EBO2);
    glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawElements(GL_LINE_STRIP, temp.i2Size, GL_UNSIGNED_INT, 0);
    glUniform4f(location, 0.0f, 1.0f, 1.0f, 1.0f);
    glDrawElements(GL_POINTS, temp.i1Size*2, GL_UNSIGNED_INT, 0);
}

//Key input
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //si presionas a muestra la estrella
    if (key == GLFW_KEY_A)
    {
        figure='a';
    }
    //si presiones s muestra la flecha
    if (key == GLFW_KEY_S)
    {
        figure = 'b';
    }
    //si presiones d muestra el circulo
    if (key == GLFW_KEY_D)
    {
        figure = 'c';
    }
}





void agrandarFigura(float &ejeX, float &ejeY, bool &crecer, int &vecesCrecer){
	if(vecesCrecer == 4){
		crecer = false;
	}
	if(vecesCrecer == 0){
		crecer=true;
	}
	
	if(crecer == true){
		ejeX = 0.7;
		ejeY = 0.7;
		vecesCrecer++;
	}
	else{
		ejeX = -0.8;
		ejeY = -0.8;
		vecesCrecer--;
	}
	
	
}


int crear_circulo(int angle, Figure &Circulo, float *&vertices_circulo2) {
    //Numero de iteraciones que tendremos
    int iterations = 360 / angle;
	
    float pointx = 0.3f, pointy = 0.0f, magnitude = 0.3f, currentAngle = 0.0, degree = 0.0;
  
    float *vertices_circulo = new float[(iterations * 3) + 6];
	vertices_circulo2 = new float[(iterations * 2) + 2];
	
    unsigned int*indices_circulo = new unsigned int[(iterations * 3) + 3];
    unsigned int*indices2_circulo = new unsigned int[iterations * 2];
    //El primer punto sera en el centro (para luego usarlo en los indices y crear triangulos)
    vertices_circulo[0] = 0.0f;
    vertices_circulo[1] = 0.0f;
    vertices_circulo[2] = 0.0f;

    //Luego nuestro segundo punto sera x=0.4
    vertices_circulo[3] = pointx;
    vertices_circulo[4] = pointy;
    vertices_circulo[5] = 0.0f;
	
	vertices_circulo2[0] = pointx;
	vertices_circulo2[1] = pointy;

    //Creando vertices
    for (int i = 6, j=2; i <= iterations * 3 + 3; i += 3, j+=2) {
        //sumamos el angulo (hasta llegar a 360)
        currentAngle += angle;
        degree = currentAngle * (3.141592653589793238463 / 180);
        pointx = magnitude * cos(degree);
        pointy = magnitude * sin(degree);
        //creamos los puntos
        vertices_circulo[i] = pointx;
        vertices_circulo[i + 1] = pointy;
        vertices_circulo[i + 2] = 0.0f;
		
		vertices_circulo2[j] = pointx;
		vertices_circulo2[j+1] = pointy;
		cout<<vertices_circulo2[j]<<" - " <<vertices_circulo2[j+1]<<endl;
    }
	cout<<endl;

    //Creando indices
    int count = 1;
    int i = 0;
    for (i; i <= iterations * 3; i += 3) {
        //siempre los triangulos usaron el punto del centro del circulo
        indices_circulo[i] = 0;
        indices_circulo[i + 1] = count;
        indices_circulo[i + 2] = count + 1;
        count++;
    }
    count = 1;
    i = 0;
    for (i; i < iterations * 2; i += 2) {
        //siempre los triangulos usaron el punto del centro del circulo
        indices2_circulo[i] = count;
        indices2_circulo[i + 1] = count + 1;
        count++;
    }

    Circulo.setVertices((iterations * 3 + 6), vertices_circulo);
    Circulo.setTriangulos((iterations * 3 + 3), indices_circulo);
    Circulo.setContorno((iterations * 2), indices2_circulo);

    Circulo.update_buffers();
	return iterations;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
