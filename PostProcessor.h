#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ����ڴ���������Ϸ��Ч
** ʹ�ÿ⣺���߿�glad��glm��ͷ�ļ�texture��SpriteRenderer��shader
** ��д�� ����������������������������
******************************************************************/


#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include "texture.h"
#include "SpriteRenderer.h"
#include "shader.h"


//����һ����������࣬Ϊ�Ͻ��ṩ���еĺ���Ч��
//����Ⱦ��Ϸ֮ǰ��Ҫ����BeginRender()
//��EndRender()������Ⱦ��Ϸ������
class PostProcessor
{
public:
	Shader PostProcessingShader;
	Texture2D  Texture;
	GLuint Width, Height;
	//����ѡ���Ч���ࣩ
	GLboolean Confuse, Chaos, Shake;
	//���캯��
	PostProcessor(Shader shader, GLuint width, GLuint height);
	//����Ⱦ��Ϸ֮ǰ׼����������֡�������
	void BeginRender();
	//��Ⱦ��Ϸ����ã�����������Ⱦ�����ݴ洢��һ�����������
	void EndRender();
	//��Ⱦ�����������ķ�ͼ(��Ϊһ����Ļ��Χ�Ĵ���)
	void Render(GLfloat time);
private:
	GLuint MSFBO, FBO;			//���ز���֡����������ͨ��FBO
	GLuint RBO;					//��Ⱦ�������
	GLuint VAO;					//�����������
	//��ʼ�����ڴ�����ı�������
	void initRenderData();
};

#endif