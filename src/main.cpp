#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../includes/file_system.hpp"
#include "../includes/camera_util.hpp"
#include "../includes/util.hpp"
#include "../includes/shader.hpp"
#include "../includes/scene01.hpp"
#include "../includes/scene_init_args.hpp"
#include "../includes/scene_update_args.hpp"
#include "../includes/text_layer.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const bool IS_FULLSCREEN = false;
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HPPEIGHT = 720;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//frame counter vars
static unsigned int frame_count = 0;
static int current_fps = 0;
static double start_time = 0;

//vars for sleep calculations
static double target_frame_rate = 60;
static double frame_start = 0;

void calcFrameRate()
{
	frame_count++;
	double elapsed = (glfwGetTime() - start_time);

	if (elapsed > 1)
	{
		current_fps = frame_count;
		start_time = glfwGetTime();
		frame_count = 0;
	}
}

void calcSleep()
{
	double wait_time = 1.0 / (target_frame_rate);
	double curr_frame_time = glfwGetTime() - frame_start;
	double dur = 1000.0 * (wait_time - curr_frame_time) + 0.5;
	int durDW = (int)dur;
	if (durDW > 0) // ensures that we don't have a dur > 0.0 which converts to a durDW of 0.
	{
        std::this_thread::sleep_for(std::chrono::milliseconds(durDW));
	}

	double frame_end = glfwGetTime();
	frame_start = frame_end;
}

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    GLFWwindow* window = NULL;
    if(IS_FULLSCREEN){
        window = glfwCreateWindow(SCR_WIDTH, SCR_HPPEIGHT, "GLDevdor04", glfwGetPrimaryMonitor(), NULL);
    }
    else {
        window = glfwCreateWindow(SCR_WIDTH, SCR_HPPEIGHT, "GLDevdor04", NULL, NULL);
    }
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // tell GLFW to capture our mouse
    if(IS_FULLSCREEN){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else{
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    }    

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // opengl scene
    CScene01 sc01;
    CSceneInitArgs scArgs = CSceneInitArgs(
        CScreenSettings(SCR_WIDTH, SCR_HPPEIGHT));
    
    sc01.Init(scArgs);    
    
    // textlayer
    CTextLayer textLayer;
    textLayer.Init(scArgs);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate fps
        calcFrameRate();

        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        CSceneUpdateArgs updArgs = CSceneUpdateArgs(
            currentFrame, deltaTime, lastFrame);

        // input
        processInput(window);

        // update scene
        sc01.Update(updArgs);

        // render scene
        sc01.SetGlStates();
        sc01.Render(updArgs);

        // render textlayer
        std::stringstream debugMsg;
	    debugMsg.setf(std::ios_base::fixed, std::ios_base::floatfield);

	    // Message BottomLeft	
	    debugMsg << "Frames/s " << current_fps;
        textLayer.RenderText(debugMsg.str(), 5.0f, 22.0f, 0.28f, glm::vec3(0.8, 0.8f, 0.8f));

        debugMsg.str(std::string());
        debugMsg << "Duration " << currentFrame;
        textLayer.RenderText(debugMsg.str(), 5.0f, 8.0f, 0.28f, glm::vec3(0.8, 0.8f, 0.8f));

        glfwSwapBuffers(window);
        glfwPollEvents();

        // ensure 60 FPS
        calcSleep();
    }
    
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}