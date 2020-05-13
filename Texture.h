#pragma once

/*******************************************************************
** This code is part of STG
** ���ܣ���ɫ�����ɺ�����
** ʹ�ÿ⣺���߿�glad
** ��д�� ��������������������
******************************************************************/

#ifndef TEXTURE_H
#define TEXTURE_H

#include"includes/glad.h"



//�����ࣺ������������������Եĳ�Ա������Դ�ļ�����ͼƬ����������
class Texture2D
{
public:
	//��������ID
	GLuint ID;
	//����ͼƬ����(����)
	GLuint Width, Height;
	//�����ʽ
	GLuint Internal_Format;	//��������ʽ
	GLuint Image_Format;	//����ͼƬ�ĸ�ʽ
	//��������
	GLuint Wrap_S;			//S���ϵĻ���ģʽ
	GLuint Wrap_T;			//T���ϵĻ���ģʽ
	GLuint Filter_Min;		//����������� < ��Ļ���أ�����ù���ģʽ
	GLuint Filter_Mag;		//����������� > ��Ļ���أ�����ù���ģʽ
	//���캯�������������
	Texture2D();
	/*---------------------------
	���ܣ���ͼƬ������������
	����1��width�������
	����2��height������ߣ�
	����3��data�����ڴ洢ͼƬ���ݣ�
	�������
	��д�ڣ�������������������
	----------------------------*/
	void Generate(GLuint width, GLuint height, unsigned char* data);

	//������󶨵���ǰ����ĵ�GL_TEXTURE_2D����
	void Bind() const;
};

#endif
