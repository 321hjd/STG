#pragma once

/*******************************************************************
** This code is part of STG
** 功能：管理游戏对象的额外属性
** 使用库：工具库glad、glm，头文件Texture、SpriteRenderer、GameObject、PowerUp
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#ifndef BALLOBJECT_H
#define BALLOBJECT_H


#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"Texture.h"
#include"SpriteRenderer.h"
#include"GameObject.h"
#include"PowerUp.h"


//BallObject 包含对象球的状态，继承了GameObject类，
//其实就是包含了一些其它属性的游戏对象
class BallObject : public GameObject
{
public:
	//球的额外属
	GLfloat Radius;					//球半径
	GLboolean Stuck;				//球是固定在板子上还是自由运动
	GLboolean Sticky, PassThrough;	//道具效果
	//构造函数
	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
	
	/*-----------------------------------------------
	功能：球的移动控制函数，在窗口外时修正球的速度和方向
	参数1：每一帧的时间
	参数2：窗口宽度
	输出：移动的2维向量
	编写于 ？？？？？？？？？？？？？？
	-----------------------------------------------*/
	glm::vec2 Move(GLfloat dt, GLuint window_width);

	/*----------------------------------------------
	功能：重置球回到初始状态
	参数1：球的坐标
	参数2：球的速度
	输出：无
	编写于 ？？？？？？？？？？？？？？
	-----------------------------------------------*/
	void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif
