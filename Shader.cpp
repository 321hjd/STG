/*******************************************************************
** This code is part of STG
** ���ܣ����ļ����ڹ�����ɫ���ı��롢�����Լ��޸�ȫ�ֱ���Uniform��ֵ
** ʹ�ÿ⣺ͷ�ļ�Shader��ϵͳ��iostream�����߿�glm
** ��д�� 2020.3.20
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
	//������ɫ��
	sVertex = glCreateShader(GL_VERTEX_SHADER);				//������ɫ������
	/*--------------------------------------
	glShaderSource(��ɫ������Դ���ַ���������Դ�룬�ݶ�ΪNULL)
	---------------------------------------*/
	glShaderSource(sVertex, 1, &vertexSource, NULL);		//����ɫ��Դ�븽�ӵ���ɫ��������	
	glCompileShader(sVertex);								//������ɫ��
	checkCompileErrors(sVertex, "VERTEX");					//��Ⲣ��ӡ��ɫ�����������Ϣ
	//Ƭ����ɫ��
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	//�������geometry��ɫ����Դ���룬ͬ��ҲҪ����������
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}
	//��ɫ�����ӵ�����
	this->ID = glCreateProgram();			//����һ���������
	glAttachShader(this->ID, sVertex);		//��������ɫ�����ӵ����������
	glAttachShader(this->ID, sFragment);	//��Ƭ����ɫ�����ӵ��ɳ��������
	if (geometrySource != nullptr)
		glAttachShader(this->ID, gShader);
	glLinkProgram(this->ID);				//���Ӷ�����ɫ����Ƭ����ɫ��
	checkCompileErrors(this->ID, "PROGRAM");//��Ⲣ��ӡ���������Ϣ
	//��ɫ�����ӵ�����֮��ɾ����ɫ��
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);
}

void Shader::SetFloat(const GLchar * name, GLfloat value, GLboolean useShader)
{
	if (useShader)							//�����Ҫ������ɫ��
		this->Use();
	/*--------------------------------
	���ܣ��޸�Uniformֵ
	����1��Uniform��������λ��
	����2���޸ĺ��ֵ
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