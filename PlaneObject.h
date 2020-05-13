#define PLANE
#ifdef PLANE

#pragma once

/*******************************************************************
** This code is part of STG
** 功能：管理飞机属性
** 使用库：工具库glad、glm
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#ifndef PLANE_OBJECT_H
#define PLANE_OBJECT_H

#include <string>

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"Texture.h"
#include"SpriteRenderer.h"
#include"GameObject.h"
#include"BulletLink.h"

//飞机的移动轨迹
enum PlaneTrack {
	P_STILL,			//静止
	P_STRAIGHT,			//直线
	P_ARC,				//弧形
	P_SECTOR			//扇形
};
//飞机类型
enum PlaneType {
	COMMON,			//普通
	INTENSIFY,		//强化
	BOSS			//关卡boss
};


//飞机类，包含飞机的状态，属性
class PlaneObject : public GameObject
{
public:
	//飞机的额外属性
	GLuint Type;					//飞机类型
	GLuint Blood;					//飞机血条
	GLuint Track;					//飞机轨迹
	GLboolean Activated;			//飞机是否被启用（被渲染在屏幕上即启用）	
	BulletLink Bullet_link;			//子弹链表
	GLuint Score;					//飞机被击毁得分
	
	GLuint Timer;					//飞机定时器，用于停止飞机
	GLuint Stayed;					//判断条件,飞机已经停过了

	//构造函数
	PlaneObject();
	PlaneObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, GLuint type);

	////检查飞机是否移动到窗口底线以下
	//void Check(GLuint window_height);

	//飞机移动轨迹设定
	glm::vec2 Move(GLfloat dt, GLuint window_height);

	////飞机在原地保持一段时间
	//void Stay_still(GLuint time);

	

};

#endif

#endif 