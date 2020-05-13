#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ�������Ϸ����Ķ�������
** ʹ�ÿ⣺���߿�glad��glm��ͷ�ļ�Texture��SpriteRenderer��GameObject��PowerUp
** ��д�� ����������������������������
******************************************************************/

#ifndef BALLOBJECT_H
#define BALLOBJECT_H


#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"Texture.h"
#include"SpriteRenderer.h"
#include"GameObject.h"
#include"PowerUp.h"


//BallObject �����������״̬���̳���GameObject�࣬
//��ʵ���ǰ�����һЩ�������Ե���Ϸ����
class BallObject : public GameObject
{
public:
	//��Ķ�����
	GLfloat Radius;					//��뾶
	GLboolean Stuck;				//���ǹ̶��ڰ����ϻ��������˶�
	GLboolean Sticky, PassThrough;	//����Ч��
	//���캯��
	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
	
	/*-----------------------------------------------
	���ܣ�����ƶ����ƺ������ڴ�����ʱ��������ٶȺͷ���
	����1��ÿһ֡��ʱ��
	����2�����ڿ��
	������ƶ���2ά����
	��д�� ����������������������������
	-----------------------------------------------*/
	glm::vec2 Move(GLfloat dt, GLuint window_width);

	/*----------------------------------------------
	���ܣ�������ص���ʼ״̬
	����1���������
	����2������ٶ�
	�������
	��д�� ����������������������������
	-----------------------------------------------*/
	void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif
