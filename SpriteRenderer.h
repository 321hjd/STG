#pragma once

/*******************************************************************
** This code is part of STG
** 功能：负责渲染工作
** 使用库：工具库glad、glm，头文件Texture、Shader
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#ifndef SPREITE_RENDERER_H
#define SPREITE_RENDERER_H

#include"includes/glad.h"
#include"includes/glm/glm.hpp"
#include"includes/glm/gtc/matrix_transform.hpp"

#include"Texture.h"
#include"Shader.h"

//对2D形状渲染的抽象类，方便渲染大量精灵
class SpriteRenderer
{
public:
	//构造函数
	SpriteRenderer(Shader &shader);
	//析构函数
	~SpriteRenderer();

	/*---------------------------------------------
	功能：使用给定的纹理渲染四边形
	参数1：纹理对象的引用
	参数2：纹理坐标
	参数3：纹理尺寸
	参数4：旋转角度
	参数5：纹理颜色
	输出：无
	编写于 ？？？？？？？？？？？？？？
	---------------------------------------------*/
	void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	Shader shader;		//渲染此四边形所用着色器
	GLuint quadVAO;		//顶点数组对象
	//初始化和配置quad的缓冲和纹理属性
	void initRenderData();
};

#endif