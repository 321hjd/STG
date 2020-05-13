#pragma once

/*****************************************************************
** This code is part of STG
** ���ܣ���Ϸ��ܣ�����������ص���Ⱦ����Ϸ���룬�򵥹�����Ϸ���룬ͬʱ�����еĴ��ڴ������Ϸ�н���
** ʹ�ÿ⣺ϵͳ��vector��tuple�����߿�glad��glfw3��ͷ�ļ�GameLevel��PowerUp
** ��д�� ����������������������������
******************************************************************/

#ifndef GAME_H
#define GAME_H

#include<vector>
#include<tuple>

#include"includes/glad.h"
#include"includes/glfw3.h"

#include"GameLevel.h"
#include"PowerUp.h"

//������Ϸ��ǰ״̬
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_LOSE
};

//ש��ķ�������
enum Direction {
	UP,				//0
	RIGHT,			//1
	DOWN,			//2
	LEFT			//3
};

//Ϊ�˸��õ���֯���룬����ײ��ص����ݶ���ΪCollision
//tuple���������ں������ض��ֵ
//typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

//��ʼ����������:��С���ٶ�
const glm::vec2 PLAYER_SIZE(100, 80);
const GLfloat PLAYER_VELOCITY(500.0f);
//��ʼ�������ԣ��ٶȺͰ뾶
const GLfloat BALL_RADIUS = 12.5f;
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

//Game�������������Ϸ��ص�״̬�͹���
//����������Ϸ��ص����ݺϲ���һ������������
//���ڷ���ÿ������Ϳɹ�����
class Game
{
public:
	GameState State;				//��Ϸ״̬
	GLboolean Keys[1024];			//����״̬
	GLuint Width, Height;
	std::vector<GameLevel> Levels;	//��Game�������һ������GameLevel������������ͬʱ�洢��ǰ����Ϸ�ؿ�
	GLuint Level;
	std::vector<PowerUp> PowerUps;	//���崢�����״̬������
	GLuint Lives;					//����ֵ
	GLuint Score;					//��Ϸ����
	GLboolean KeysProcessed[1024];	//��ʾ������İ���
	//����/��������
	Game(GLuint width, GLuint height);
	~Game();
	//��Ϸ��ʼ�����������е���ɫ��/����/�ؿ���
	void Init();
	//��Ϸѭ��
	void ProcessInput(GLfloat dt);	//��������
	void Update(GLfloat dt);		//������Ϸ����״̬
	void Render();					//��Ⱦ
	void DoCollisions();			//��ײ���,���ô˺���������֯��ײ������
	//����
	void ResetLevel();
	void ResetPlayer();
	//�������
	//void SpawnPowerUps(GameObject &block);	//�ڸ���ש��λ�����ɵ���
	//void UpdatePowerUps(GLfloat dt);		//����ǰ���б�����ĵ���
};

#endif
