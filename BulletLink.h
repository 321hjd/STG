#define Bul_Link
#ifdef Bul_Link

#pragma once

/*******************************************************************
** This code is part of STG
** 功能：管理子弹链表
** 使用库：工具库glad、glm
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#ifndef BULLET_LINK_H
#define BULLET_LINK_H

#include <vector>
#include <list>

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"Texture.h"
#include"SpriteRenderer.h"
#include"BulletObject.h"

//子弹压入的时间间隔
const GLfloat FIRE_INTERVAL = 0.18f;

//子弹容器，用来存储维护子弹对象
class BulletLink
{
public:
	std::vector<BulletObject> Bullets;										//链表存储子弹
	GLfloat fire_interval;													//压入子弹的时间间隔
	
	//构造函数
	BulletLink();

	void Add_bullet(glm::vec2 self_pos, Texture2D sprite, GLuint type, GLfloat direction);		//一旦开火则向链表push子弹
	void Pop_bullet();																			//更新用：检测链表最后一颗子弹，如果子弹IsDestroyed则pop出这颗子弹
	void Draw_all(SpriteRenderer &renderer);													//渲染链表内所有的子弹
	void Move_all(GLfloat dt, GLuint window_width, GLuint window_height);						//移动链表内所有的子弹
	void Clear_all();																			//清除链表所有子弹
	GLboolean Timer(GLfloat dt);																//更新射击计时器
};

#endif


#endif 