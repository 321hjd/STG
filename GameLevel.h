#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ�����ͼ��عؿ�����
** ʹ�ÿ⣺ϵͳ��vector�����߿�glad��glm��ͷ�ļ�GameObject��SpriteRenderer��ResourceManager
** ��д�� ����������������������������
******************************************************************/

#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include<vector>

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"PlaneObject.h"
#include"SpriteRenderer.h"
#include"ResourceManager.h"


// GameLevel������Tiles��Ϊͻ�ƹؿ���һ����
///��Ӳ�̼���/���ּ�����������ܡ�
class GameLevel
{
public:
	std::vector<PlaneObject> Planes;
	//���캯��
	GameLevel(){}

	/*--------------------------------------
	����:���ļ����عؿ�����
	����1���ؿ��ļ�·��
	����2���ؿ����
	����3���ؿ��߶�
	�������
	��д�� ����������������������������
	--------------------------------------*/
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);

	//��Ⱦ�ɻ�
	void Draw(SpriteRenderer &renderer);
	//����Ƿ����еķɻ����ݻ�
	GLboolean IsComplete();
	//�ƶ��ɻ�
	void Move(GLfloat dt, GLuint window_height, GLuint window_width);
	//Ϊ�ɻ�����ӵ�
	void Add_bullet();

	////���ɻ��Ƿ��ѵ��ﴰ�ڵײ�
	//void Check(GLuint window_height);
	

private:
	/*------------------------------------
	���ܣ���ʼ���ɻ�����
	����1���Ѽ��صĹؿ��ļ�����
	����2���ؿ����
	����3���ؿ��߶�
	�������
	��д�� ����������������������������
	------------------------------------*/
	void init(std::vector<std::vector<GLuint>>tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif 