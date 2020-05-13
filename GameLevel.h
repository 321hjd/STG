#pragma once

/*******************************************************************
** This code is part of STG
** 功能：管理和加载关卡数据
** 使用库：系统库vector，工具库glad、glm，头文件GameObject、SpriteRenderer、ResourceManager
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include<vector>

#include"includes/glad.h"
#include"includes/glm/glm.hpp"

#include"PlaneObject.h"
#include"SpriteRenderer.h"
#include"ResourceManager.h"


// GameLevel将所有Tiles作为突破关卡的一部分
///从硬盘加载/呈现级别的主机功能。
class GameLevel
{
public:
	std::vector<PlaneObject> Planes;
	//构造函数
	GameLevel(){}

	/*--------------------------------------
	功能:从文件加载关卡数据
	参数1：关卡文件路径
	参数2：关卡宽度
	参数3：关卡高度
	输出：无
	编写于 ？？？？？？？？？？？？？？
	--------------------------------------*/
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);

	//渲染飞机
	void Draw(SpriteRenderer &renderer);
	//检查是否所有的飞机被摧毁
	GLboolean IsComplete();
	//移动飞机
	void Move(GLfloat dt, GLuint window_height, GLuint window_width);
	//为飞机添加子弹
	void Add_bullet();

	////检测飞机是否已到达窗口底部
	//void Check(GLuint window_height);
	

private:
	/*------------------------------------
	功能：初始化飞机数据
	参数1：已加载的关卡文件数据
	参数2：关卡宽度
	参数3：关卡高度
	输出：无
	编写于 ？？？？？？？？？？？？？？
	------------------------------------*/
	void init(std::vector<std::vector<GLuint>>tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif 