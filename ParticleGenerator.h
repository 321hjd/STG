#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ�������Ч��������
** ʹ�ÿ⣺ϵͳ��vector�����߿�glad��glm��ͷ�ļ�Shader��Texture��GameObject
** ��д�� ����������������������������
******************************************************************/


#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include "includes/glad.h"
#include "includes/glm/glm.hpp"

#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

//�������Ժ�״̬
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;
	//ʹ�ó�ʼ���б��ʼ������
	Particle() :Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

//���ӷ�������
class ParticleGenerator
{
public:
	//���캯��
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	
	/*--------------------------------------------
	���ܣ�������������
	����1��ÿһ֡��ʱ��
	����2����Ϸ����
	����3�����������ӵ�����
	����4�����ӵ�ƫ����
	�������
	��д�� ����������������������������
	---------------------------------------------*/
	void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	
	//��Ⱦ��������
	void Draw();
private:
	//����״̬
	std::vector<Particle> particles;	//��������
	GLuint amount;						//��������
	//��Ⱦ״̬
	Shader shader;						//��Ⱦ������ʹ�õ���ɫ��
	Texture2D texture;					//��Ⱦ������ʹ�õ�����
	GLuint VAO;							//�����������
	
	void init();

	/*-------------------------------------
	���ܣ����ص�һ������������
	��������
	�������һ���������ӵ�λ������
	��д�� ����������������������������
	--------------------------------------*/
	GLuint firstUnusedParticle();
	
	/*------------------------------------
	���ܣ���������
	����1�������������������
	����2����Ϸ����
	����3������ƫ����
	�������
	��д�� ����������������������������
	-------------------------------------*/
	void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};


#endif