#pragma once

#include "..\Libraries\include\GLFW\glfw3.h"

typedef struct Vector2
{
    float x;
    float y;
};

class Input
{
public:
    Input(GLFWwindow* window);
    
public:
    static Vector2 mousePosition;
};