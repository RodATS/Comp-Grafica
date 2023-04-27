#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
	
	
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
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
    // ---------------------------------------
	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
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
	
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	
	
    // link shaders
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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
	//menor el ángulo se ve mejor la circunferencia
	int angle = 10;
    int numberOfMovements = 360/angle;
	
	float degree = 0.0, angleCambio = 0.0, radius = 0.4f;
	
	float *vertices = new float[((numberOfMovements+1) * 3)+1];
	float *vertices2 = new float[((numberOfMovements+1) * 3)+1];
	
	
	unsigned int *indices = new unsigned int[(numberOfMovements * 3)];
	
    unsigned int*indices2_circulo = new unsigned int[(numberOfMovements+1)];


	
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;

    vertices[3] = 0.4f;
    vertices[4] = 0.0f;
    vertices[5] = 0.0f;
	
	vertices2[0] = 0.0f;
    vertices2[1] = 0.0f;
    vertices2[2] = 0.0f;

    vertices2[3] = 0.4f;
    vertices2[4] = 0.0f;
    vertices2[5] = 0.0f;
	
	
	for (int i = 6; i < ((numberOfMovements+1) * 3)+1; i += 3) {
        //sumamos el angulo (hasta llegar a 360)
        angleCambio += angle;
        degree = angleCambio * (3.141592653589793238463 / 180);
        
		//ejex
        vertices[i] = radius * cos(degree);
		//ejey
        vertices[i + 1] = radius * sin(degree);
		//ejez
        vertices[i + 2] = 0.0f;
		
		//el otro array
		//ejex
        vertices2[i] = radius * cos(degree);
		//ejey
        vertices2[i + 1] = radius * sin(degree);
		//ejez
        vertices2[i + 2] = 0.0f;
    }

	

    //Creando indices
    int aumenta = 1;
    int i = 0;
    for (i; i < (numberOfMovements * 3)+1; i += 3) {
        //siempre los triangulos usaron el punto del centro del circulo
        indices[i] = 0;
        indices[i + 1] = aumenta;
        indices[i + 2] = aumenta + 1;
        aumenta++;
    }
	
	
	aumenta = 1;
	int aux=3;
    i = 0;
    for (i; i < (numberOfMovements) ; i += 1) {
        //siempre los triangulos usaron el punto del centro del circulo
        indices2_circulo[i] = aumenta;
		aumenta++;
    }
	indices2_circulo[i]=1;



    unsigned int VBOs[2], VAOs[2], EBOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    glGenBuffers(2, EBOs);
	
	
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, (((numberOfMovements+1) * 3)+1) * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ((numberOfMovements * 3)+1) * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	
	glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, (((numberOfMovements+1) * 3)+1) * sizeof(float), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (numberOfMovements+1) * sizeof(unsigned int), indices2_circulo, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
   
   glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
	glPointSize(8.0f);
	glLineWidth(3.0f);
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]); 
		
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation,0.5f, 0.5f, 0.5f, 0.0f);
		glDrawElements(GL_TRIANGLE_STRIP,((numberOfMovements * 3)+1), GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(VAOs[1]);
		glUniform4f(vertexColorLocation,1.0f, 1.0f, 1.0f, 0.0f);
		glDrawElements(GL_LINE_STRIP,numberOfMovements+1 , GL_UNSIGNED_INT, 0);
		glUniform4f(vertexColorLocation,1.0f, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_POINTS, 0, (numberOfMovements+1));
		
		
		/*
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]); 
        glDrawElements(GL_TRIANGLES, ((numberOfMovements * 3)+1), GL_UNSIGNED_INT, 0);
		
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glLineWidth((GLfloat)4);
		glUniform4f(location, 0.0f, 0.5f, 0.5f, 1.0f);
		glDrawElements(GL_LINES, ((numberOfMovements * 2)+1), GL_UNSIGNED_INT, 0);  */ 

        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(2, EBOs);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
