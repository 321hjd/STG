#pragma once

/*******************************************************************
** This code is part of STG
** 功能：后期处理，管理游戏特效
** 使用库：工具库glad、glm，头文件texture、SpriteRenderer、shader
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/


#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include "texture.h"
#include "SpriteRenderer.h"
#include "shader.h"


//这是一个后处理程序类，为断接提供所有的后处理效果
//在渲染游戏之前需要调用BeginRender()
//和EndRender()这类渲染游戏后工作。
class PostProcessor
{
public:
	Shader PostProcessingShader;
	Texture2D  Texture;
	GLuint Width, Height;
	//处理选项（特效种类）
	GLboolean Confuse, Chaos, Shake;
	//构造函数
	PostProcessor(Shader shader, GLuint width, GLuint height);
	//在渲染游戏之前准备后处理器的帧缓冲操作
	void BeginRender();
	//渲染游戏后调用，它把所有渲染的数据存储到一个纹理对象中
	void EndRender();
	//渲染后处理器纹理四分图(作为一个屏幕包围的大精灵)
	void Render(GLfloat time);
private:
	GLuint MSFBO, FBO;			//多重采样帧缓冲对象和普通的FBO
	GLuint RBO;					//渲染缓冲对象
	GLuint VAO;					//顶点数组对象
	//初始化后期处理的四边形纹理
	void initRenderData();
};

#endif