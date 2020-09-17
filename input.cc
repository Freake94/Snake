
bool key_pressed [GLFW_KEY_LAST];
bool key_released[GLFW_KEY_LAST];
bool key_down    [GLFW_KEY_LAST];

void InputUpdate() {
    if(key_pressed[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(window, true);
    
    memset(key_pressed,  0, sizeof(key_pressed));
    memset(key_released, 0, sizeof(key_released));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*
    key_pressed[key]  = action == GLFW_PRESS;
    key_released[key] = action == GLFW_RELEASE;
    key_down[key]    |= action == GLFW_PRESS;
    if(action == GLFW_RELEASE) key_down[key] = false;
    */
    if(action == GLFW_PRESS) {
        key_pressed[key] = true;
        key_down[key]    = true;
    }
    else if(action == GLFW_RELEASE) {
        key_released[key] = true;
        key_down[key]     = false;
    }
}