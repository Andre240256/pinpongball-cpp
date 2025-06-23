#include<iostream>
#include<vector>
#include<math.h>
#include<GLFW/glfw3.h>
const float PI = 3.1415926535f;
float screenHeight= 600.0f;
float screenWidth = 800.0f;

GLFWwindow *StartGLFW()
{
    if(!glfwInit())
    {
        std::cerr<<"erro ao inicializar"<<std::endl;
        return nullptr;
    }

    GLFWwindow* window= glfwCreateWindow(800, 600,"ping pong ball",NULL,NULL);
    if(window==NULL)
    {
        std::cerr<<"erro ao criar janela"<< std::endl;
        glfwTerminate();
        return nullptr;
    }

    return window;
}

void DrawCircle(float centerX, float centerY, float radius, int res)
{
    glBegin(GL_TRIANGLE_FAN);

    //adiciona a cor branca como a ser utilizada
    glColor3f(1.0f,1.0f,1.0f);
    glVertex2d(centerX,centerY);
    for(int i=0; i<=res; i++)
    {
        float angle=PI*2.0f*float(i)/float(res);
        float x=centerX + radius*cos(angle);
        float y=centerY + radius*sin(angle);
        glVertex2d(x,y);
    }
    glEnd();
}

int main()
{
    GLFWwindow* window= StartGLFW();
    if(!window)
        return -1;

    float radius=50.0f;
    int res=100;

    std::vector<float> position={screenWidth/2.0f, screenHeight-radius};
    std::vector<float> velocity={0.0f,0.0f};

    glfwMakeContextCurrent(window);

    //realiza a fução todo o frame
    while(!glfwWindowShouldClose(window))
    {
        //limpa o buffer da tela colorida
        glClear(GL_COLOR_BUFFER_BIT);
        glfwMakeContextCurrent(window);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f,screenWidth,0.0f,screenHeight,-1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        DrawCircle(position[0], position[1], radius, res);

        // Delta(x)/Delta(t)
        position[0]+=velocity[0];
        position[1]+=velocity[1];
        // Delta^2(x)/Delta^2(t)
        velocity[1]-=0.001f;
        velocity[0]+=0.0001f;
        
        //colisao com coeficiente de restituição 0.95
        if(position[1]<radius || position[1]>screenHeight-radius)
            velocity[1]*=-0.95f;
        if(position[0]>screenWidth-radius || position[0]<radius)
            velocity[0]*=-0.95f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    glfwTerminate();
    return 0;
}