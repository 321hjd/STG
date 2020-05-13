/*******************************************************************
** This code is part of STG
** 功能：管理所有的文件资源，包括着色器、纹理、音频、关卡数据、存档数据
** 使用库：系统库iostream、sstream、fstream，工具库stb_image，头文件ResourceManager
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#include "ResourceManager.h"

#include<iostream>
#include<sstream>
#include<fstream>

//通过定义STB_IMAGE_IMPLEMENTATION，预处理器会修改头文件，让其只包含相关的函数定义源码，相当于把这个头文件变为一个 .cpp 文件
#define STB_IMAGE_IMPLEMENTATION
#include"includes/stb_image.h"


//初始化静态变量
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader>	 ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const GLchar * vShaderFile, const GLchar * fShaderFile, const GLchar * gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);//加载着色器资源并以name为key存入map映射
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar * file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	//删除所有的着色器
	for (auto iter : Shaders)			//for range——容器遍历，auto标识符可以根据初始值推断数据类型
		glDeleteProgram(iter.second.ID);//iter.first是key，second是value
	//删除所有的纹理
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar * vShaderFile, const GLchar * fShaderFile, const GLchar * gShaderFile)
{
	//从文件路径检索着色器资源代码
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		//打开文件
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		//从文件的缓冲内容读取数据流
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		//关闭文件
		vertexShaderFile.close();
		fragmentShaderFile.close();
		//将stream对象转化为string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//如果geometry着色器文件路径存在，也加载一个geometry着色器
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	//将string对象转为字符串,返回字符串首地址
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = nullptr;
	if (gShaderFile != nullptr)
		gShaderCode = geometryCode.c_str();
	//创建着色器对象
	Shader shader;
	//将着色器源码送进Compile里面链接并编译
	shader.Compile(vShaderCode, fShaderCode, gShaderCode != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar * file, GLboolean alpha)
{
	//创建纹理对象
	Texture2D texture;
	if (alpha)					//如果是要混合两张纹理，显示在上面的一张需要修改格式
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	//加载图片
	int width, height, nrChannels;
	/*-----------------------------
	stbi_load函数
	功能：加载图片
	参数1：图片文件位置
	参数2、3、4：图片宽、高、颜色通道个数
	------------------------------*/
	unsigned char* image = stbi_load(file, &width, &height, &nrChannels, 0);//图像数据
	//生成纹理
	texture.Generate(width, height, image);
	//生成纹理后，释放图像内存
	stbi_image_free(image);
	return texture;
}
