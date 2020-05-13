/*******************************************************************
** This code is part of STG.
** 功能：创建窗口和整个游戏的框架
** 使用库：头文件Game、ResourceManager，系统库iostream，工具库glad、glfw3
** 编写于 ？？？？？？？？？
******************************************************************/


#include"includes/glad.h"
#include"includes/glfw3.h"

#include"Game.h"
#include"ResourceManager.h"

#include<iostream>

//键盘输入
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
/*-------------------------------------------------------
功能：改变帧缓冲大小，当窗口被第一次显示的时候framebuffer_size_callback也会被调用
参数1：GLFW对象
参数2、3：新窗口宽、高
---------------------------------------------------------*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


//屏幕属性
const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
	//初始化和窗口建立
	glfwInit();											//初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		//主版本号设为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		//次版本号设为3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//核心模式
#ifdef _RESIZE_WINDOW_
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);			//取消注释可以禁用改变窗口大小
#endif

	/*-----------------------------
	glfwCreateWindow函数
	功能：创建窗口
	参数：窗口宽、高，标题，*，*
	返回值：GLFWwindow对象
	------------------------------*/
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//创建窗口完成，通知GLFW将我们窗口的上下文设置为当前线程的主上下文

	//调用OpenGL函数前需要初始化GLAD
	//给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//注册键盘输入函数
	glfwSetKeyCallback(window, key_callback);
	//注册改变窗口大小的帧缓冲函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//OpenGL 配置
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);		//设置视口
	glEnable(GL_CULL_FACE);								//使用面剔除
	glEnable(GL_BLEND);									//使用混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//混合颜色的源因子，目标因子=(1-源因子)

	//初始化游戏
	Breakout.Init();

	//渲染时间
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	//引擎
	while (!glfwWindowShouldClose(window))
	{
		//计算每一帧的渲染时间
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//检查有没有触发什么事件(比如键盘输入、鼠标移动等)
		glfwPollEvents();

		//管理用户输入
		Breakout.ProcessInput(deltaTime);

		//更新游戏状态
		Breakout.Update(deltaTime);

		//渲染
		//glClearColor为状态设置函数，设置清空屏幕所用的颜色
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear为状态使用函数，清空屏幕颜色缓冲，它接受一个缓冲位(Buffer Bit)
		glClear(GL_COLOR_BUFFER_BIT);

		Breakout.Render();

		//交换颜色缓冲
		glfwSwapBuffers(window);
	}

	//删除所有的加载的资源
	ResourceManager::Clear();

	//渲染结束，释放资源
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//按ESC键退出
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//按下键则将标志设为true，释放则重置为false
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

//帧缓冲大小函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	/*---------------------------------------
	glViewport函数
	功能：设置视口
	参数1、2：设置窗口左下角位置
	参数3、4：渲染窗口的宽、高（单位为像素）
	----------------------------------------*/
	glViewport(0, 0, width, height);
}