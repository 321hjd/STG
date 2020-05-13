/*******************************************************************
** This code is part of STG
** ���ܣ�������Ⱦ����
** ʹ�ÿ⣺ϵͳ��iostream��ͷ�ļ�SpriteRenderer
** ��д�� ����������������������������
******************************************************************/

#include<iostream>

#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader & shader)
{
	this->shader = shader;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	//����任
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);//ȷ����ʼ�����ǵ�λ����

	//һ��˳���� ����->��ת->λ�ƣ����ھ��������Ǵ��ҵ������Է�����
	//���Ƚ���λ��
	model = glm::translate(model, glm::vec3(position, 0.0f));

	//Ȼ����ת,��ת�����������ϽǶ��㣬�����Ƚ�����������������ת
	//���漰����ת����
	model = glm::translate(model, glm::vec3(size.x * 0.5, size.y * 0.5, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0f));

	//�������
	model = glm::scale(model, glm::vec3(size, 1.0f));

	//�޸Ķ�����ɫ����uniformֵ
	shader.SetMatrix4("model", model);

	//��Ⱦ�ı�������
	//��ɫ
	this->shader.SetVector3f("spriteColor", color);
	//��������Ԫ
	glActiveTexture(GL_TEXTURE0);
	//��������GL_TEXTURE_2D����
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
	//����VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		//λ��		//����
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);		//���ɶ����������
	glGenBuffers(1, &VBO);						//���ɶ��㻺�����

	glBindBuffer(GL_ARRAY_BUFFER, VBO);			//�󶨶��㻺�������GL_ARRAY_BUFFER

	/*----------------------------------------------------
	glBufferData����
	���ܣ���֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	����1����Ŀ�껺�������:���㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ����
	����2��ָ���������ݵĴ�С(���ֽ�Ϊ��λ)��һ���򵥵�sizeof������������ݴ�С����
	����3��ϣ�����͵�ʵ������
	����4��ָ��������ϣ���Կ���ι�����������ݣ���������Ϊÿ�λ�ͼ���ı�ͼ���ģʽ
	-----------------------------------------------------*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);			//�󶨶����������
	glEnableVertexAttribArray(0);				//����λ������ֵΪ0�Ķ������ԣ�Ĭ�Ϲرգ�λ�����Զ�������ɫ��Դ���У��Ƕ���λ�ú�������ɵ�������

	/*-------------------------------------------------
	glVertexAttribPointer����
	���ܣ����ö�������
	����1��ָ��Ҫ���õĶ�������λ��ֵ���ڶ�����ɫ���ж������
	����2��ָ���������ԵĴ�С������������һ��vec4���ʴ�СΪ4
	����3��ָ���������ͣ�GLSL��vec*��Ϊfloat�ͣ�
	����4���Ƿ�ϣ�����ݱ���׼��
	����5��������Stride��������������������֮��ļ��
	����6��(void*)��ǿ������ת������ʾλ�������ڻ�������ʼλ�õ�ƫ����
	---------------------------------------------------*/
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//���VBO��quadVAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
