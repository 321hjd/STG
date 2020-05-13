#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ�������Ⱦ����
** ʹ�ÿ⣺���߿�glad��glm��ͷ�ļ�Texture��Shader
** ��д�� ����������������������������
******************************************************************/

#ifndef SPREITE_RENDERER_H
#define SPREITE_RENDERER_H

#include"includes/glad.h"
#include"includes/glm/glm.hpp"
#include"includes/glm/gtc/matrix_transform.hpp"

#include"Texture.h"
#include"Shader.h"

//��2D��״��Ⱦ�ĳ����࣬������Ⱦ��������
class SpriteRenderer
{
public:
	//���캯��
	SpriteRenderer(Shader &shader);
	//��������
	~SpriteRenderer();

	/*---------------------------------------------
	���ܣ�ʹ�ø�����������Ⱦ�ı���
	����1��������������
	����2����������
	����3������ߴ�
	����4����ת�Ƕ�
	����5��������ɫ
	�������
	��д�� ����������������������������
	---------------------------------------------*/
	void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	Shader shader;		//��Ⱦ���ı���������ɫ��
	GLuint quadVAO;		//�����������
	//��ʼ��������quad�Ļ������������
	void initRenderData();
};

#endif