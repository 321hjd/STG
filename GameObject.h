#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ����������Ϸ��������
** ʹ�ÿ⣺ͷ�ļ�Texture��SpriteRenderer�����߿�glad��glm
** ��д�� ����������������������������
******************************************************************/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"Texture.h"
#include"SpriteRenderer.h"


//��Ϸ�����࣬���ڱ�������Ϸ������ص���С״̬
class GameObject
{
public:
	//��Ϸ���������
	glm::vec2 Position, Size, Velocity; //��Ϸ�����λ�á���С���ٶ�
	glm::vec3 Color;					//��ɫ
	GLfloat Rotation;					//��ת�Ƕ�
	GLboolean Destroyed;				//�Ƿ񱻴ݻ�
	//����
	Texture2D Sprite;
	//���캯��
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	//������
	virtual void Draw(SpriteRenderer &renderer);
};

#endif

