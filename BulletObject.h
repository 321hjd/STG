#define BOBJ
#ifdef BOBJ

#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ������ӵ�����
** ʹ�ÿ⣺���߿�glad��glm
** ��д�� ����������������������������
******************************************************************/

#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H

#include <string>

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"Texture.h"
#include"SpriteRenderer.h"
#include"GameObject.h"

//�ӵ����ƶ��켣
enum BulletTrack {
	STRAIGHT,		//ֱ��
	ARC,			//����
	SECTOR			//����
};
//�ӵ�����
enum BulletType {
	SINGLE,			//������
	DOUBLE			//˫����
};


//�ӵ��࣬�����ӵ���״̬������
class BulletObject : public GameObject
{
public:
	//�ӵ��Ķ�������
	GLuint Type;					//�ӵ�����
	GLfloat Direction;				//�ӵ��ƶ�����
	GLuint Track;					//�ӵ��켣
	GLuint Damage;					//�ӵ�����
	//���캯��
	BulletObject();
	BulletObject(glm::vec2 pos, Texture2D sprite, GLuint type, GLfloat direction);

	////����ӵ��Ƿ��ƶ���������
	//void Check(GLuint window_width, GLuint window_height);

	//�ӵ��ƶ��߼��趨
	glm::vec2 Move(GLfloat dt, GLuint window_width, GLuint window_height);
};

#endif

#endif 