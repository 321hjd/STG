/*******************************************************************
** This code is part of STG.
** ���ܣ��������ں�������Ϸ�Ŀ��
** ʹ�ÿ⣺ͷ�ļ�Game��ResourceManager��ϵͳ��iostream�����߿�glad��glfw3
** ��д�� ������������������
******************************************************************/


#include"includes/glad.h"
#include"includes/glfw3.h"

#include"Game.h"
#include"ResourceManager.h"

#include<iostream>

//��������
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
/*-------------------------------------------------------
���ܣ��ı�֡�����С�������ڱ���һ����ʾ��ʱ��framebuffer_size_callbackҲ�ᱻ����
����1��GLFW����
����2��3���´��ڿ���
---------------------------------------------------------*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


//��Ļ����
const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
	//��ʼ���ʹ��ڽ���
	glfwInit();											//��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		//���汾����Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		//�ΰ汾����Ϊ3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//����ģʽ
#ifdef _RESIZE_WINDOW_
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);			//ȡ��ע�Ϳ��Խ��øı䴰�ڴ�С
#endif

	/*-----------------------------
	glfwCreateWindow����
	���ܣ���������
	���������ڿ��ߣ����⣬*��*
	����ֵ��GLFWwindow����
	------------------------------*/
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//����������ɣ�֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������

	//����OpenGL����ǰ��Ҫ��ʼ��GLAD
	//��GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//ע��������뺯��
	glfwSetKeyCallback(window, key_callback);
	//ע��ı䴰�ڴ�С��֡���庯��
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//OpenGL ����
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);		//�����ӿ�
	glEnable(GL_CULL_FACE);								//ʹ�����޳�
	glEnable(GL_BLEND);									//ʹ�û��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//�����ɫ��Դ���ӣ�Ŀ������=(1-Դ����)

	//��ʼ����Ϸ
	Breakout.Init();

	//��Ⱦʱ��
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	//����
	while (!glfwWindowShouldClose(window))
	{
		//����ÿһ֡����Ⱦʱ��
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//�����û�д���ʲô�¼�(����������롢����ƶ���)
		glfwPollEvents();

		//�����û�����
		Breakout.ProcessInput(deltaTime);

		//������Ϸ״̬
		Breakout.Update(deltaTime);

		//��Ⱦ
		//glClearColorΪ״̬���ú��������������Ļ���õ���ɫ
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClearΪ״̬ʹ�ú����������Ļ��ɫ���壬������һ������λ(Buffer Bit)
		glClear(GL_COLOR_BUFFER_BIT);

		Breakout.Render();

		//������ɫ����
		glfwSwapBuffers(window);
	}

	//ɾ�����еļ��ص���Դ
	ResourceManager::Clear();

	//��Ⱦ�������ͷ���Դ
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//��ESC���˳�
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//���¼��򽫱�־��Ϊtrue���ͷ�������Ϊfalse
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Breakout.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
		{
			Breakout.Keys[key] = GL_FALSE;
			Breakout.KeysProcessed[key] = GL_FALSE;
		}
	}
}

//֡�����С����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	/*---------------------------------------
	glViewport����
	���ܣ������ӿ�
	����1��2�����ô������½�λ��
	����3��4����Ⱦ���ڵĿ��ߣ���λΪ���أ�
	----------------------------------------*/
	glViewport(0, 0, width, height);
}