#pragma once

/*******************************************************************
** This code is part of STG
** 功能：本文件用于管理着色器的编译、链接以及修改全局变量Uniform的值
** 使用库：系统库string，工具库glad、glm
** 编写于 2020.3.20
******************************************************************/

#ifndef SHADER_H
#define SHADER_H

#include <string>

#include"includes/glad.h"
#include"includes/glm/glm.hpp"
#include"includes/glm/gtc/type_ptr.hpp"


/*-------------------------------------------
类名：着色器类
功能：建立着色器对象、从文件中加载着色器代码、
		链接程序和着色器、检测打印加载、编译错误信息
---------------------------------------------*/
class Shader
{
public:
	//程序ID
	GLuint ID;
	//构造函数
	Shader() {}
	//激活着色器（在更改着色器的uniform值必须在激活着色器之后）
	Shader &Use();

	/*---------------------------------------------
	功能：从source文件中获取源代码，生成、编译并链接着色器
	参数1：顶点着色器着色器源码的文件路径
	参数2：片段着色器着色器源码的文件路径
	参数3：几何着色器着色器源码的文件路径（可选择，默认为nullptr）
	输出：无
	编写于 2020.3.20 
	----------------------------------------------*/
	void Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);
	
	/*---------------------------------------------
	功能:工具函数，用于修改全局变量Uniform的值
	参数1：name（Uniform变量的名称）
	参数2：value或xyz（修改后的值）
	参数3：useShader（更改前是否需要先激活着色器）
	输出：无
	注：以下均为同样功能的赋值函数
	编写于 2020.3.20
	----------------------------------------------*/
	void SetFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
	void SetInteger(const GLchar *name, GLint value, GLboolean useShader = false);
	void SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
	void SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	void SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
	void SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
private:
	/*-----------------------------------------------------------
	功能：检查着色器编译或程序链接是否成功，并在控制台打印错误信息
	参数1：object（着色器对象）
	参数2：type（着色器类型）
	输出：无
	------------------------------------------------------------*/
	void checkCompileErrors(GLuint object, std::string type);
};

#endif
