#define PLANE
#ifdef PLANE

#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ�����ɻ�����
** ʹ�ÿ⣺���߿�glad��glm
** ��д�� ����������������������������
******************************************************************/

#ifndef PLANE_OBJECT_H
#define PLANE_OBJECT_H

#include <string>

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"Texture.h"
#include"SpriteRenderer.h"
#include"GameObject.h"
#include"BulletLink.h"

//�ɻ����ƶ��켣
enum PlaneTrack {
	P_STILL,			//��ֹ
	P_STRAIGHT,			//ֱ��
	P_ARC,				//����
	P_SECTOR			//����
};
//�ɻ�����
enum PlaneType {
	COMMON,			//��ͨ
	INTENSIFY,		//ǿ��
	BOSS			//�ؿ�boss
};


//�ɻ��࣬�����ɻ���״̬������
class PlaneObject : public GameObject
{
public:
	//�ɻ��Ķ�������
	GLuint Type;					//�ɻ�����
	GLuint Blood;					//�ɻ�Ѫ��
	GLuint Track;					//�ɻ��켣
	GLboolean Activated;			//�ɻ��Ƿ����ã�����Ⱦ����Ļ�ϼ����ã�	
	BulletLink Bullet_link;			//�ӵ�����
	GLuint Score;					//�ɻ������ٵ÷�
	
	GLuint Timer;					//�ɻ���ʱ��������ֹͣ�ɻ�
	GLuint Stayed;					//�ж�����,�ɻ��Ѿ�ͣ����

	//���캯��
	PlaneObject();
	PlaneObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, GLuint type);

	////���ɻ��Ƿ��ƶ������ڵ�������
	//void Check(GLuint window_height);

	//�ɻ��ƶ��켣�趨
	glm::vec2 Move(GLfloat dt, GLuint window_height);

	////�ɻ���ԭ�ر���һ��ʱ��
	//void Stay_still(GLuint time);

	

};

#endif

#endif 