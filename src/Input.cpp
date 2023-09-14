#include "Input.h"
#include "EngineWindow.h"

Vector2 Input::mousePosition;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Input::mousePosition.x = xpos - (engine::window::windowWidth / 2.0f);
    Input::mousePosition.y = ypos - (engine::window::windowHeight / 2.0f);
}

Input::Input(GLFWwindow* window)
{
    glfwSetCursorPosCallback(window, mouse_callback);
}
