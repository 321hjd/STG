#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ������ı���Ⱦ
** ʹ�ÿ⣺ϵͳ��map��ͷ�ļ�Texture��Shader�����߿�glad��glm
** ��д�� ����������������������������
******************************************************************/

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include "includes/glad.h"
#include "includes/glm/glm.hpp"

#include "Texture.h"
#include "Shader.h"


//������ʹ��FreeType���ص��ַ���ص�����״̬��Ϣ
struct Character {
	GLuint TextureID;	//��ʶ���������ID���
	glm::ivec2 Size;	//�����С
	glm::ivec2 Bearing;	//�ӻ��ߵ��������/������ƫ����
	GLuint Advance;		//ǰ������һ�����ε�ˮƽƫ����
};

//���ص���������ʾ���ı��ĳ��ֳ�����
//���ص�һ���壬���䴦��Ϊ�ַ��б�
class TextRenderer
{
public:
	std::map<GLchar, Character> Characters;	//�����ɵ��ַ���Ž�һ��ӳ��
	Shader TextShader;						//������Ⱦ���ε���ɫ��
	//���캯��
	TextRenderer(GLuint width, GLuint height);
	
	/*-----------------------------------------
	���ܣ��Ӹ�������Ԥ�����ַ��б�
	����1�������ļ�·��
	����2�������С
	�������
	��д�� ����������������������������
	-----------------------------------------*/
	void Load(std::string font, GLuint fontSize);
	
	/*-----------------------------------------
	���ܣ��ñ���õ��ַ��б���Ⱦ�ı�
	����1����Ҫ��Ⱦ���ı�
	����2���ı�x����
	����3���ı�y����
	����4���ı����Ŵ�С
	����5���ı���ɫ
	�������
	��д�� ����������������������������
	------------------------------------------*/
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	GLuint VAO, VBO;	//��Ⱦ���趥�����
};


#endif