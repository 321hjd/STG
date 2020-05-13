#pragma once

/*******************************************************************
** This code is part of STG
** 功能：管理游戏道具
** 使用库：系统库string，工具库glad、glm，头文件GameObject
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/


#ifndef POWER_UP_H
#define POWER_UP_H
#include <string>

#include "includes/glad.h"
#include "includes/glm/glm.hpp"

#include "GameObject.h"

//道具的大小
const glm::vec2 SIZE(60, 20);
//道具出现的移动速度
const glm::vec2 VELOCITY(0.0f, 150.0f);


//道具类，看作是具有一些额外属性的GameObject
//包括道具的持续作用时间和种类
class PowerUp : public GameObject
{
public:
	//道具属性
	std::string Type;		//道具种类
	GLfloat Duration;		//持续作用时间
	GLboolean Activated;	//是否作用
	//构造函数
	PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
		:GameObject(position, SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() {}
};

#endif