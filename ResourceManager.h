#pragma once

/*******************************************************************
** This code is part of STG
** 功能：管理所有的文件资源，包括着色器、纹理、音频、关卡数据、存档数据
** 使用库：系统库string，map，工具库glad，头文件Texture、Shader
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include<map>		//map为关联性容器，可以自动建立key-value的对应
#include<string>

#include"includes/glad.h"

#include"Texture.h"
#include"Shader.h"


/*----------------------------------------------
一个静态的单例ResourceManager类
功能：加载纹理和着色器的函数。每个加载纹理或着色器
	也被存储为字符串引用处理。所有的功能和资源都是
	静态的，同时定义了一个公共的构造函数
-----------------------------------------------*/
class ResourceManager
{
public:
	//资源存储
	static std::map<std::string, Shader>	Shaders;
	static std::map<std::string, Texture2D> Textures;
	
	/*---------------------------------------------
	功能：从文件中加载着色器程序
	参数1：顶点着色器文件路径
	参数2：片段着色器文件路径
	参数3：几何着色器文件路径（可选择，默认为nullptr）
	参数4：着色器名称
	输出：Shader对象
	编写于 ？？？？？？？？？？？？？？
	----------------------------------------------*/
	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	
	/*---------------------------------------------
	功能：检索存储的着色器
	参数：着色器名称
	输出：Shader对象
	编写于 ？？？？？？？？？？？？？？
	----------------------------------------------*/
	static Shader GetShader(std::string name);

	/*---------------------------------------------
	功能：从文件中加载纹理
	参数1：纹理路径
	参数2：透明选项
	参数3：纹理名称
	输出：Texture2D对象
	编写于 ？？？？？？？？？？？？？？
	----------------------------------------------*/
	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
	
	/*---------------------------------------------
	功能：检索存储的纹理
	参数：纹理名称
	输出：Texture2D对象
	编写于 ？？？？？？？？？？？？？？
	----------------------------------------------*/
	static Texture2D GetTexture(std::string name);
	
	//删除着色器和纹理
	static void Clear();
private:
	//私有构造函数
	ResourceManager() {	}

	/*---------------------------------------------
	功能：从文件中加载并生成着色器程序
	参数1：顶点着色器文件路径
	参数2：片段着色器文件路径
	参数3：几何着色器文件路径（可选择，默认为nullptr）
	输出：Shader对象
	编写于 ？？？？？？？？？？？？？？
	----------------------------------------------*/
	static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	
	/*---------------------------------------------
	功能：从文件中加载并生成纹理
	参数1：纹理路径
	参数2：透明选项
	输出：Texture2D对象
	编写于 ？？？？？？？？？？？？？？
	----------------------------------------------*/
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif
