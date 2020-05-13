#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ����ļ����ڹ�����ɫ���ı��롢�����Լ��޸�ȫ�ֱ���Uniform��ֵ
** ʹ�ÿ⣺ϵͳ��string�����߿�glad��glm
** ��д�� 2020.3.20
******************************************************************/

#ifndef SHADER_H
#define SHADER_H

#include <string>

#include"includes/glad.h"
#include"includes/glm/glm.hpp"
#include"includes/glm/gtc/type_ptr.hpp"


/*-------------------------------------------
��������ɫ����
���ܣ�������ɫ�����󡢴��ļ��м�����ɫ�����롢
		���ӳ������ɫ��������ӡ���ء����������Ϣ
---------------------------------------------*/
class Shader
{
public:
	//����ID
	GLuint ID;
	//���캯��
	Shader() {}
	//������ɫ�����ڸ�����ɫ����uniformֵ�����ڼ�����ɫ��֮��
	Shader &Use();

	/*---------------------------------------------
	���ܣ���source�ļ��л�ȡԴ���룬���ɡ����벢������ɫ��
	����1��������ɫ����ɫ��Դ����ļ�·��
	����2��Ƭ����ɫ����ɫ��Դ����ļ�·��
	����3��������ɫ����ɫ��Դ����ļ�·������ѡ��Ĭ��Ϊnullptr��
	�������
	��д�� 2020.3.20 
	----------------------------------------------*/
	void Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);
	
	/*---------------------------------------------
	����:���ߺ����������޸�ȫ�ֱ���Uniform��ֵ
	����1��name��Uniform���������ƣ�
	����2��value��xyz���޸ĺ��ֵ��
	����3��useShader������ǰ�Ƿ���Ҫ�ȼ�����ɫ����
	�������
	ע�����¾�Ϊͬ�����ܵĸ�ֵ����
	��д�� 2020.3.20
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
	���ܣ������ɫ���������������Ƿ�ɹ������ڿ���̨��ӡ������Ϣ
	����1��object����ɫ������
	����2��type����ɫ�����ͣ�
	�������
	------------------------------------------------------------*/
	void checkCompileErrors(GLuint object, std::string type);
};

#endif
