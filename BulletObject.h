#define BOBJ
#ifdef BOBJ

#pragma once

/*******************************************************************
** This code is part of STG
** 功能：管理子弹属性
** 使用库：工具库glad、glm
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H

#include <string>

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"Texture.h"
#include"SpriteRenderer.h"
#include"GameObject.h"

//子弹的移动轨迹
enum BulletTrack {
	STRAIGHT,		//直线
	ARC,			//弧形
	SECTOR			//扇形
};
//子弹类型
enum BulletType {
	SINGLE,			//单火线
	DOUBLE			//双火线
};


//子弹类，包含子弹的状态，属性
class BulletObject : public GameObject
{
public:
	//子弹的额外属性
	GLuint Type;					//子弹类型
	GLfloat Direction;				//子弹移动方向
	GLuint Track;					//子弹轨迹
	GLuint Damage;					//子弹火力
	//构造函数
	BulletObject();
	BulletObject(glm::vec2 pos, Texture2D sprite, GLuint type, GLfloat direction);

	////检查子弹是否移动到窗口外
	//void Check(GLuint window_width, GLuint window_height);

	//子弹移动逻辑设定
	glm::vec2 Move(GLfloat dt, GLuint window_width, GLuint window_height);
};

#endif

#endif 