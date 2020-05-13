/*******************************************************************
** This code is part of STG
** 功能：负责渲染工作
** 使用库：系统库iostream，头文件SpriteRenderer
** 编写于 ？？？？？？？？？？？？？？
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
	//矩阵变换
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);//确保初始化的是单位矩阵

	//一般顺序是 缩放->旋转->位移，由于矩阵运算是从右到左，所以反过来
	//首先进行位移
	model = glm::translate(model, glm::vec3(position, 0.0f));

	//然后旋转,旋转规则是绕左上角顶点，这里先将顶点移至中心再旋转
	//不涉及到旋转操作
	model = glm::translate(model, glm::vec3(size.x * 0.5, size.y * 0.5, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0f));

	//最后缩放
	model = glm::scale(model, glm::vec3(size, 1.0f));

	//修改顶点着色器的uniform值
	shader.SetMatrix4("model", model);

	//渲染四边形纹理
	//颜色
	this->shader.SetVector3f("spriteColor", color);
	//激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	//绑定纹理至GL_TEXTURE_2D对象
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
	//配置VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		//位置		//纹理
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);		//生成顶点数组对象
	glGenBuffers(1, &VBO);						//生成顶点缓冲对象

	glBindBuffer(GL_ARRAY_BUFFER, VBO);			//绑定顶点缓冲对象至GL_ARRAY_BUFFER

	/*----------------------------------------------------
	glBufferData函数
	功能：把之前定义的顶点数据复制到缓冲的内存中
	参数1：是目标缓冲的类型:顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上
	参数2：指定传输数据的大小(以字节为单位)用一个简单的sizeof计算出顶点数据大小就行
	参数3：希望发送的实际数据
	参数4：指定了我们希望显卡如何管理给定的数据，这里设置为每次画图不改变图像的模式
	-----------------------------------------------------*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);			//绑定顶点数组对象
	glEnableVertexAttribArray(0);				//启用位置属性值为0的顶点属性（默认关闭，位置属性定义在着色器源码中，是顶点位置和纹理组成的向量）

	/*-------------------------------------------------
	glVertexAttribPointer函数
	功能：配置顶点属性
	参数1：指定要配置的顶点属性位置值（在顶点着色器中定义过）
	参数2：指定顶点属性的大小，顶点属性是一个vec4，故大小为4
	参数3：指定数据类型（GLSL中vec*均为float型）
	参数4：是否希望数据被标准化
	参数5：步长（Stride），即连续顶点属性组之间的间隔
	参数6：(void*)型强制类型转换，表示位置数据在缓冲中起始位置的偏移量
	---------------------------------------------------*/
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//解绑VBO和quadVAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
