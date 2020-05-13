#pragma once

/*******************************************************************
** This code is part of STG
** 功能：管理基本游戏对象属性
** 使用库：头文件Texture、SpriteRenderer，工具库glad、glm
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"Texture.h"
#include"SpriteRenderer.h"


//游戏对象类，用于保存与游戏对象相关的最小状态
class GameObject
{
public:
	//游戏对象的属性
	glm::vec2 Position, Size, Velocity; //游戏对象的位置、大小、速度
	glm::vec3 Color;					//颜色
	GLfloat Rotation;					//旋转角度
	GLboolean Destroyed;				//是否被摧毁
	//精灵
	Texture2D Sprite;
	//构造函数
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	//画精灵
	virtual void Draw(SpriteRenderer &renderer);
};

#endif

