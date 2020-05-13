/*******************************************************************
** This code is part of STG
** 功能：本文件用于管理着色器的编译、链接以及修改全局变量Uniform的值
** 使用库：头文件Shader，系统库iostream，工具库glm
** 编写于 2020.3.20
******************************************************************/

#include"Shader.h"
#include"includes/glm/glm.hpp"

#include<iostream>

Shader & Shader::Use()
{
	glUseProgram(this->ID);
	return *this;
}

void Shader::Compile(const GLchar * vertexSource, const GLchar * fragmentSource, const GLchar * geometrySource)
{
	GLuint sVertex, sFragment, gShader;
	//顶点着色器
	sVertex = glCreateShader(GL_VERTEX_SHADER);				//创建着色器对象
	/*--------------------------------------
	glShaderSource(着色器对象，源码字符串数量，源码，暂定为NULL)
	---------------------------------------*/
	glShaderSource(sVertex, 1, &vertexSource, NULL);		//将着色器源码附加到着色器对象上	
	glCompileShader(sVertex);								//编译着色器
	checkCompileErrors(sVertex, "VERTEX");					//检测并打印着色器编译错误信息
	//片段着色器
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	//如果给了geometry着色器资源代码，同样也要创建并编译
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}
	//着色器链接到程序
	this->ID = glCreateProgram();			//创建一个程序对象
	glAttachShader(this->ID, sVertex);		//将顶点着色器附加到程序对象上
	glAttachShader(this->ID, sFragment);	//将片段着色器附加到成程序对象上
	if (geometrySource != nullptr)
		glAttachShader(this->ID, gShader);
	glLinkProgram(this->ID);				//链接顶点着色器和片段着色器
	checkCompileErrors(this->ID, "PROGRAM");//检测并打印编译错误信息
	//着色器链接到程序之后删除着色器
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);
}

void Shader::SetFloat(const GLchar * name, GLfloat value, GLboolean useShader)
{
	if (useShader)							//如果需要激活着色器
		this->Use();
	/*--------------------------------
	功能：修改Uniform值
	参数1：Uniform变量所在位置
	参数2：修改后的值
	---------------------------------*/
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetInteger(const GLchar * name, GLint value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetVector2f(const GLchar * name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::SetVector2f(const GLchar * name, const glm::vec2 & value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::SetVector3f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::SetVector3f(const GLchar * name, const glm::vec3 & value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}

void Shader::SetVector4f(const GLchar * name, const glm::vec4 & value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const GLchar * name, const glm::mat4 & matrix, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout<<"| ERROR::SHADER: Compile-time error: Type: "<<type<<"\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}