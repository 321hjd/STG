#define Bul_Link
#ifdef Bul_Link

#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ������ӵ�����
** ʹ�ÿ⣺���߿�glad��glm
** ��д�� ����������������������������
******************************************************************/

#ifndef BULLET_LINK_H
#define BULLET_LINK_H

#include <vector>
#include <list>

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"Texture.h"
#include"SpriteRenderer.h"
#include"BulletObject.h"

//�ӵ�ѹ���ʱ����
const GLfloat FIRE_INTERVAL = 0.18f;

//�ӵ������������洢ά���ӵ�����
class BulletLink
{
public:
	std::vector<BulletObject> Bullets;										//����洢�ӵ�
	GLfloat fire_interval;													//ѹ���ӵ���ʱ����
	
	//���캯��
	BulletLink();

	void Add_bullet(glm::vec2 self_pos, Texture2D sprite, GLuint type, GLfloat direction);		//һ��������������push�ӵ�
	void Pop_bullet();																			//�����ã�����������һ���ӵ�������ӵ�IsDestroyed��pop������ӵ�
	void Draw_all(SpriteRenderer &renderer);													//��Ⱦ���������е��ӵ�
	void Move_all(GLfloat dt, GLuint window_width, GLuint window_height);						//�ƶ����������е��ӵ�
	void Clear_all();																			//������������ӵ�
	GLboolean Timer(GLfloat dt);																//���������ʱ��
};

#endif


#endif 