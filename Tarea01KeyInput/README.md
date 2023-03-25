//esto se va a modificar <br>
glfwSetKeyCallback(window, key_callback);

y su función era:<br>
//funcion key_callback responde con el scape esto se modifica, cierra la ventana
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
	
//modificatoria
int state = glfwGetKey(window, GLFW_KEY_ESCAPE); <br>
if (state == GLFW_PRESS)<br>
{<br>
  glfwSetWindowShouldClose(window, GLFW_TRUE);<br>
}<br>
glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);<br>
//aqui acaba
