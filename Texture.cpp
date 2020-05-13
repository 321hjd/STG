/*******************************************************************
** This code is part of STG
** ���ܣ���ɫ�����ɺ�����
** ʹ�ÿ⣺ϵͳ��iostream��ͷ�ļ�Texture
** ��д�� ��������������������
******************************************************************/

#include<iostream>

#include "Texture.h"


Texture2D::Texture2D() :
	Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Mag(GL_LINEAR)
{
	glGenTextures(1, &this->ID);	//�������������������洢��ID
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char * data)
{
	this->Width = width;
	this->Height = height;
	//��������
	glBindTexture(GL_TEXTURE_2D, this->ID);			//��һ��2D�������

	/*-------------------------------------------
	glTexImage2D����
	���ܣ���������
	����1��ָ������Ŀ������ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ��
	�����������ͬһ��Ŀ���ϵ������κΰ󶨵�GL_TEXTURE_1D��
	GL_TEXTURE_3D���������ܵ�Ӱ��
	����2��ָ���༶��Զ����ļ���
	����3��ϣ��������洢��ʽ
	����4��5��ͼ�����
	����6������0
	����7��8������Դͼ��ʽ���������ͣ�������RGBֱ����ͼ�񲢴���Ϊ
			char(byte)����
	����9��������ͼ������-data
	--------------------------------------------*/
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);

	/*-----------------------------------------------
	glTexParameter ����:glTexParameteri glTexParameterfv
	���ܣ�Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	����1��ָ������Ŀ��
	����2��ָ�����õ�ѡ���Ӧ�õ������᣺
		���ƣ�����2 ָ��S��T�᣻����3 ָ�����Ʒ�ʽ������)
		���ˣ�����2 ָ���Ŵ����С������3 ���˷�ʽ
	-------------------------------------------------*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);
	//����������
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
