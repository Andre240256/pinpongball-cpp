#include<iostream>
#include<vector>
#include<math.h>
#include<GLFW/glfw3.h>
#include<ranges>
const float PI = 3.1415926535f;
float screenHeight= 800.0f;
float screenWidth = 600.0f;
float screenRatio=screenWidth/screenHeight;

//calcula a distancia entre dois pontos no plano
float Distancia(float x1, float y1, float x2, float y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

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


class circle
{
    public:

    std::vector<float> position;
    std::vector<float> velocity;
    float radius;
    float mass;
    
    circle(std::vector<float> position_arg, std::vector<float> velocity_arg, float radius_arg )
    {
        position=position_arg;
        velocity=velocity_arg;
        radius=radius_arg;
        mass=radius*radius;
    }

    //atualiza a velocidade do circulo
    void accelerate(float x, float y)
    {
        velocity[0]+=x;
        velocity[1]+=y;
    }

    //atualiza a posição do circulo
    void newPosic()
    {
        position[0]+=velocity[0];
        position[1]+=velocity[1];
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

    bool operator==(const circle& other) const
    {
        return position[0]==other.position[0] && radius==other.radius && position[1]==other.position[1];
    }
};

int main()
{
    GLFWwindow* window= StartGLFW();
    if(!window)
        return -1;

    float radius=50.0f;
    int res=100;

    std::vector<circle> objetos={
        circle(std::vector<float> {300.0f, 400.0f}, std::vector<float>{0.0f, 0.0f},50.0f),
        circle(std::vector<float> {500.0f, 700.0f}, std::vector<float> {0.0f, 0.0f}, 100.0f)
    };
    //especifica em qual janela vai ser desenhada
    glfwMakeContextCurrent(window);

    //configura o padrao da tela trocando da porcentagem pelos pixels
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f,screenWidth,0.0f,screenHeight,-1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //realiza a fução todo o frame
    while(!glfwWindowShouldClose(window))
    {
        //limpa o buffer da tela colorida
        glClear(GL_COLOR_BUFFER_BIT);
        glfwMakeContextCurrent(window);

        
        for(circle &obj : objetos)
        {
            obj.accelerate(0.0001f, -0.001f);
            obj.newPosic();
            obj.DrawCircle(obj.position[0],obj.position[1],obj.radius,100);

            //colisao com coeficiente de restituição 0.95
            if(obj.position[1]<obj.radius || obj.position[1]>screenHeight-obj.radius)
                obj.velocity[1]*=-0.95f;
            if(obj.position[0]>screenWidth-obj.radius || obj.position[0]<obj.radius)
                obj.velocity[0]*=-0.95f;

            //olhar apenas para um intervalo do vetor
            auto itr=std::ranges::find(objetos,obj);
            auto subrange=std::ranges::subrange(itr,objetos.end());
            
            //procurando colisões entre bolinhas
            float aux1,aux2;
            for(circle &obj2 : subrange)
                if(Distancia(obj.position[0],obj.position[1],obj2.position[0],obj2.position[1])<obj.radius+obj2.radius)
                {
                    //colisão dectada 
                    //correção de velocidade em x considerando um coef. restituição=1
                    aux1=obj.velocity[0];
                    aux2=obj2.velocity[0];
                    obj.velocity[0]=(float(2)*obj2.mass*aux2+aux1*(obj.mass-obj2.mass))/(obj.mass+obj2.mass);
                    obj2.velocity[0]=(float(2)*obj.mass*aux1+aux2*(obj2.mass-obj.mass))/(obj2.mass+obj.mass);

                    //correção de velocidade em y considerando um coef. restituição=1
                    aux1=obj.velocity[1];
                    aux2=obj2.velocity[1];
                    obj.velocity[1]=(float(2)*obj2.mass*aux2+aux1*(obj.mass-obj2.mass))/(obj.mass+obj2.mass);
                    obj2.velocity[1]=(float(2)*obj.mass*aux1+aux2*(obj2.mass-obj.mass))/(obj2.mass+obj.mass);

                }

        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    glfwTerminate();
    return 0;
}