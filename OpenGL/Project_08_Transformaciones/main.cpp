#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cmath>
#include "transf.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


const char* fragmentShaderSource = "#version 330 core\n"
"layout(location=0) out vec4 FragColor;\n"
"uniform vec4 u_color;"
"void main()\n"
"{\n"
"   FragColor=u_color;\n"
"}\n\0";


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


struct Figure {
    float *vertices;
    unsigned int *indicesTriangulos;
    unsigned int* indicesContorno;
    int vSize, i1Size, i2Size;
    unsigned int VBO, EBO1, EBO2;
	//VBO PARA LOS VERTICES
	//EBO1 PARA LIS INDICES DEL CONTORNO
	//EBO2 PARA LOS INDICES DE LOS TRIANGULOS 
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


    void update_buffers() {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(float), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, i2Size* sizeof(int), indicesContorno, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, i1Size* sizeof(int), indicesTriangulos, GL_STATIC_DRAW);
    }


    void rot_x(float angle) 
	{ 
		Transform.Rotacion_x(angle, vertices, vSize); 
	}
	
    void rot_y(float angle) 
	{ 
		Transform.Rotacion_y(angle, vertices, vSize); 
	}
	
    void rot_z(float angle) 
	{
		Transform.Rotacion_z(angle, vertices, vSize);
	}
	
    void traslation(float tx, float ty, float tz)
	{
		Transform.Translation(tx, ty, tz, vertices, vSize); 
	
	}
	
    void scale(float sx, float sy, float sz)
	{
		Transform.Scale(sx, sy, sz, vertices, vSize); 
	}

    void print() 
	{
        for (int i = 0; i < vSize; i = i+3) 
		{
            cout << "x: " << vertices[i] << " y: " << vertices[i + 1] << " z: " << vertices[i + 2]<<endl;
        }
    }
};


    void create_circle(int angle, Figure &circulo);

    void create_figure(Figure& temp, int location);
	
	void create_points(Figure& temp, int location);
	
	void create_lines(Figure& temp, int location);
	
	
	

char figure = 'f';	

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    float vertices_estrella[] = {
         0.0f, 0.4f, 0.0f,  // top
         -0.17, 0.02f, 0.0f,  // bottom
         0.3f, -0.27f, 0.0f,   // right

         -0.46f, 0.19f, 0.0f, //left
         0.46f, 0.19f, 0.0f, //right
         -0.01f, -0.08f, 0.0f, //bottom

         0.0f, 0.4f, 0.0f,  // top
         -0.3, -0.26f, 0.0f,  // left
         0.17f, 0.02f, 0.0f,   // bottom

         - 0.1f, 0.19f, 0.0f, //otro
         0.1f, 0.19f, 0.0f //otro
    };

    unsigned int indices_estrella[] = {  
        0, 1, 2,  
        3, 4, 5,   
        6, 7, 8
    };

    unsigned int indices2_estrella[] = {  
        0,10,
        10,4,
        4,8,
        8,2,
        2,5,
        5,7,
        7,1,
        1,3,
        3,9,
        9,0 
    };

    float vertices_flecha[] = {
        //top triangle
         0.0f, 0.4f, 0.0f, //top
         0.4f, 0.1f, 0.0f, //right
         -0.4, 0.1f, 0.0f, //left

         //rectangle (2 triangles)
         -0.2f, 0.1f, 0.0f,
         0.2f, 0.1f, 0.0f,
         -0.2f, -0.2f, 0.0f,
         0.2f, -0.2f, 0.0f
    };

    unsigned int indices_flecha[] = {
        0, 1, 2,
        3, 4, 5,
        5, 6, 4
    };

    unsigned int indices2_flecha[] = {
        0,1,
        1,4,
        4,6,
        6,5,
        5,3,
        3,2,
        2,0
    };
	
	
	int angle = 30;
    int numberOfMovements = 360/angle;
	float degree = 0.0, angleCambio = 0.0, radius = 0.70f;
	float traslacion_circulo[numberOfMovements*2];
	
	
	
	for (int i = 0; i < numberOfMovements*2; i += 2) {
        //sumamos el angulo (hasta llegar a 360)
        angleCambio += angle;
        degree = angleCambio * (3.141592653589793238463 / 180);
        
		//ejex
        traslacion_circulo[i] = radius * cos(degree);
		//ejey
        traslacion_circulo[i + 1] = radius * sin(degree);
		
		cout<<traslacion_circulo[i]<<" "<<traslacion_circulo[i+1]<<endl;

    }
	
	

    unsigned int VAO, VBO[3], EBO[6];

    glGenVertexArrays(1, &VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(6, EBO);
    

    glBindVertexArray(VAO);
    //VBO[0] = Estrella
    //VBO[1] = Flecha
    //VBO[2] = Circulo
 
    Figure Estrella(VBO[0], EBO[0], EBO[1]);
    Estrella.setVertices(sizeof(vertices_estrella) / sizeof(float), vertices_estrella);
    Estrella.setTriangulos(sizeof(indices_estrella) / sizeof(unsigned int), indices_estrella);
    Estrella.setContorno(sizeof(indices2_estrella) / sizeof(unsigned int), indices2_estrella);

    Figure Flecha(VBO[1], EBO[2], EBO[3]);
    Flecha.setVertices(sizeof(vertices_flecha) / sizeof(float), vertices_flecha);
    Flecha.setTriangulos(sizeof(indices_flecha) / sizeof(unsigned int), indices_flecha);
    Flecha.setContorno(sizeof(indices2_flecha) / sizeof(unsigned int), indices2_flecha);

    Figure Circulo(VBO[2], EBO[4], EBO[5]);


    Estrella.update_buffers();
    Flecha.update_buffers();

    int location = glGetUniformLocation(shaderProgram, "u_color");
    if (location == -1) {
        printf("Error al conseguir uniform");
        exit(1);
    }


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glfwSetKeyCallback(window, keyCallback);


    create_circle(5, Circulo);
    float currColor = 0.1f;
    float adder = 0.005f;
	
	Estrella.traslation(0,-0.15, 0);
	Estrella.scale(0.5,0.5,0);
	
	Flecha.traslation(0,-1.2,0);
	Flecha.scale(0.5,0.5,0);
	
	int flag=-1;
	float prevx=0.0;
	float prevy=0.0;
	
	float prev_scale_x=1.0;
	float prev_scale_y=1.0;


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);

        double  timeValue = glfwGetTime();
        float color_cambiante = static_cast<float>(sin(timeValue) / 2.0 + 0.5);


        glUniform4f(location, 1.0f, 0.5f, 0.2f, 1.0f);


        glLineWidth((GLfloat)5);


        glPointSize((GLfloat)5);

        switch (figure)
        {

        case 'f': {
			glUniform4f(location, color_cambiante, 0.0f, 0.0f, 1.0f);
            create_figure(Estrella, location);
			int time=((int)glfwGetTime())%numberOfMovements;
			int time2=((int)glfwGetTime())%5;
			
			
			//cout<<time<<endl;
			//cout<<traslacion_circulo[time*2]<<" "<<traslacion_circulo[(time*2)+1]<<endl;
            //Estrella.rot_z(0.02);	
			
			Estrella.traslation(traslacion_circulo[time*2]-prevx,traslacion_circulo[(time*2)+1]-prevy,0);
			prevx=traslacion_circulo[time*2];
			prevy=traslacion_circulo[(time*2)+1];

		
			
            Estrella.update_buffers();
			glUniform4f(location, 0.0f, color_cambiante, 0.0f, 1.0f);
			
			create_figure(Circulo, location);
			Circulo.scale(1/prev_scale_x,1/prev_scale_y,0);
			Circulo.scale(0.5+(0.1*time2),0.5+(0.1*time2),0);
			prev_scale_x=0.5+(0.1*time2);
			prev_scale_y=0.5+(0.1*time2);
			
			Circulo.update_buffers();
        }
            break;
			
		case 'p': {
			glUniform4f(location, color_cambiante, 0.0f, 0.0f, 1.0f);
            create_points(Estrella, location);
			int time=((int)glfwGetTime())%numberOfMovements;
			int time2=((int)glfwGetTime())%5;
			
			//cout<<time<<endl;
			//cout<<traslacion_circulo[time*2]<<" "<<traslacion_circulo[(time*2)+1]<<endl;
            //Estrella.rot_z(0.02);	
			
			Estrella.traslation(traslacion_circulo[time*2]-prevx,traslacion_circulo[(time*2)+1]-prevy,0);
			prevx=traslacion_circulo[time*2];
			prevy=traslacion_circulo[(time*2)+1];
			
			
            Estrella.update_buffers();
			glUniform4f(location, 0.0f, color_cambiante, 0.0f, 1.0f);
			create_points(Circulo, location);
			Circulo.scale(1/prev_scale_x,1/prev_scale_y,0);
			Circulo.scale(0.5+(0.1*time2),0.5+(0.1*time2),0);
			prev_scale_x=0.5+(0.1*time2);
			prev_scale_y=0.5+(0.1*time2);
			Circulo.update_buffers();
        }
            break;
		case 'l': {
			glUniform4f(location, color_cambiante, 0.0f, 0.0f, 1.0f);
            create_lines(Estrella, location);
			int time=((int)glfwGetTime())%numberOfMovements;
			int time2=((int)glfwGetTime())%11;
			
			//cout<<time<<endl;
			//cout<<traslacion_circulo[time*2]<<" "<<traslacion_circulo[(time*2)+1]<<endl;
            //Estrella.rot_z(0.02);	
			
			Estrella.traslation(traslacion_circulo[time*2]-prevx,traslacion_circulo[(time*2)+1]-prevy,0);
			prevx=traslacion_circulo[time*2];
			prevy=traslacion_circulo[(time*2)+1];
			
			
            Estrella.update_buffers();
			glUniform4f(location, 0.0f, color_cambiante, 0.0f, 1.0f);
			create_lines(Circulo, location);
			Circulo.scale(1/prev_scale_x,1/prev_scale_y,0);
			Circulo.scale(0.5+(0.1*time2),0.5+(0.1*time2),0);
			prev_scale_x=0.5+(0.1*time2);
			prev_scale_y=0.5+(0.1*time2);
			Circulo.update_buffers();
        }
            break;
			
		 case 'r': {
			glUniform4f(location, color_cambiante, 0.0f, 0.0f, 1.0f);
            create_figure(Flecha, location);
			int time=((int)glfwGetTime())%numberOfMovements;
			int time2=((int)glfwGetTime())%5;
			
			
			//cout<<time<<endl;
			//cout<<traslacion_circulo[time*2]<<" "<<traslacion_circulo[(time*2)+1]<<endl;
            Flecha.rot_z(0.02);	
			
            Flecha.update_buffers();
			glUniform4f(location, 0.0f, color_cambiante, 0.0f, 1.0f);
			create_figure(Circulo, location);
			Circulo.scale(1/prev_scale_x,1/prev_scale_y,0);
			Circulo.scale(0.5+(0.1*time2),0.5+(0.1*time2),0);
			prev_scale_x=0.5+(0.1*time2);
			prev_scale_y=0.5+(0.1*time2);
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


void create_figure(Figure &temp, int location) {
    glBindBuffer(GL_ARRAY_BUFFER, temp.VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.EBO1);
    glDrawElements(GL_TRIANGLES, temp.i1Size, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.EBO2);
    glUniform4f(location, 0.0f, 0.0f, 0.0f, 1.0f);
    glDrawElements(GL_LINES, temp.i2Size, GL_UNSIGNED_INT, 0);
    glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f);
    glDrawElements(GL_POINTS, temp.i1Size*2, GL_UNSIGNED_INT, 0);
}

void create_points(Figure &temp, int location) {
    glBindBuffer(GL_ARRAY_BUFFER, temp.VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.EBO1);
    //glDrawElements(GL_TRIANGLES, temp.i1Size, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.EBO2);
    glUniform4f(location, 0.0f, 0.0f, 0.0f, 1.0f);
    //glDrawElements(GL_LINES, temp.i2Size, GL_UNSIGNED_INT, 0);
    glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f);
    glDrawElements(GL_POINTS, temp.i1Size*2, GL_UNSIGNED_INT, 0);
}

void create_lines(Figure &temp, int location) {
    glBindBuffer(GL_ARRAY_BUFFER, temp.VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.EBO1);
    //glDrawElements(GL_TRIANGLES, temp.i1Size, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.EBO2);
    glUniform4f(location, 0.0f, 0.0f, 0.0f, 1.0f);
    glDrawElements(GL_LINES, temp.i2Size, GL_UNSIGNED_INT, 0);
    glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f);
    //glDrawElements(GL_POINTS, temp.i1Size*2, GL_UNSIGNED_INT, 0);
}



//Key input
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//puntos
	if (key == GLFW_KEY_P)
    {
        figure = 'p';
    }
	//lineas
	if (key == GLFW_KEY_L)
    {
        figure = 'l';
    }
	//figuras
    if (key == GLFW_KEY_F)
    {
        figure='f';
    }
	//rotacion 2
	if (key == GLFW_KEY_R)
    {
        figure = 'r';
    }

	
	
}

void create_circle(int angle, Figure &Circulo) {
    int iterations = 360 / angle;
    float pointx = 0.4f, pointy = 0.0f, magnitude = 0.4f, currentAngle = 0.0, degree = 0.0;

    float *vertices_circulo = new float[(iterations * 3) + 6];
    unsigned int*indices_circulo = new unsigned int[(iterations * 3) + 3];
    unsigned int*indices2_circulo = new unsigned int[iterations * 2];

    vertices_circulo[0] = 0.0f;
    vertices_circulo[1] = 0.0f;
    vertices_circulo[2] = 0.0f;


    vertices_circulo[3] = pointx;
    vertices_circulo[4] = pointy;
    vertices_circulo[5] = 0.0f;


    for (int i = 6; i <= iterations * 3 + 3; i += 3) {
        currentAngle += angle;
        degree = currentAngle * (3.141592653589793238463 / 180);
		
        pointx = magnitude * cos(degree);
        pointy = magnitude * sin(degree);

        vertices_circulo[i] = pointx;
        vertices_circulo[i + 1] = pointy;
        vertices_circulo[i + 2] = 0.0f;
    }


    int count = 1;
    int i = 0;
    for (i; i <= iterations * 3; i += 3) 
	{
        indices_circulo[i] = 0;
        indices_circulo[i + 1] = count;
        indices_circulo[i + 2] = count + 1;
        count++;
    }
    count = 1;
    i = 0;
    for (i; i < iterations * 2; i += 2) 
	{
        indices2_circulo[i] = count;
        indices2_circulo[i + 1] = count + 1;
        count++;
    }

    Circulo.setVertices((iterations * 3 + 6), vertices_circulo);
    Circulo.setTriangulos((iterations * 3 + 3), indices_circulo);
    Circulo.setContorno((iterations * 2), indices2_circulo);

    Circulo.update_buffers();
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
