#include <iostream>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <string.h>

#include "types.h"
#include "particle.h"
#include "game.h"

static GLFWwindow* window;

#include "input.cc"
#include "render.cc"
#include "particle.cc"
#include "game.cc"

int Init() {
    if(!glfwInit()) return -1;
    window = glfwCreateWindow(640, 640, "Snake", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER, 0.0);
    glEnable(GL_ALPHA_TEST);
    return 0;
}

int main() {
    if(Init() != 0) return -1;
    Run();
    glfwTerminate();
}