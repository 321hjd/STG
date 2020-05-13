#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ�������Ϸ����
** ʹ�ÿ⣺ϵͳ��string�����߿�glad��glm��ͷ�ļ�GameObject
** ��д�� ����������������������������
******************************************************************/


#ifndef POWER_UP_H
#define POWER_UP_H
#include <string>

#include "includes/glad.h"
#include "includes/glm/glm.hpp"

#include "GameObject.h"

//���ߵĴ�С
const glm::vec2 SIZE(60, 20);
//���߳��ֵ��ƶ��ٶ�
const glm::vec2 VELOCITY(0.0f, 150.0f);


//�����࣬�����Ǿ���һЩ�������Ե�GameObject
//�������ߵĳ�������ʱ�������
class PowerUp : public GameObject
{
public:
	//��������
	std::string Type;		//��������
	GLfloat Duration;		//��������ʱ��
	GLboolean Activated;	//�Ƿ�����
	//���캯��
	PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
		:GameObject(position, SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() {}
};

#endif