#pragma once

/*****************************************************************
** This code is part of STG
** 功能：游戏框架，包含所有相关的渲染和游戏代码，简单管理游戏代码，同时将所有的窗口代码从游戏中解耦
** 使用库：系统库vector、tuple，工具库glad、glfw3，头文件GameLevel、PowerUp
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#ifndef GAME_H
#define GAME_H

#include<vector>
#include<tuple>

#include"includes/glad.h"
#include"includes/glfw3.h"

#include"GameLevel.h"
#include"PowerUp.h"

//跟踪游戏当前状态
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_LOSE
};

//砖块的方向向量
enum Direction {
	UP,				//0
	RIGHT,			//1
	DOWN,			//2
	LEFT			//3
};

//为了更好地组织代码，把碰撞相关的数据定义为Collision
//tuple容器可用于函数返回多个值
//typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

//初始化挡板属性:大小和速度
const glm::vec2 PLAYER_SIZE(100, 80);
const GLfloat PLAYER_VELOCITY(500.0f);
//初始化球属性：速度和半径
const GLfloat BALL_RADIUS = 12.5f;
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

//Game类包含所有与游戏相关的状态和功能
//将所有与游戏相关的数据合并到一个单独的类中
//易于访问每个组件和可管理性
class Game
{
public:
	GameState State;				//游戏状态
	GLboolean Keys[1024];			//按键状态
	GLuint Width, Height;
	std::vector<GameLevel> Levels;	//在Game类中添加一个持有GameLevel变量的容器。同时存储当前的游戏关卡
	GLuint Level;
	std::vector<PowerUp> PowerUps;	//定义储存道具状态的容器
	GLuint Lives;					//生命值
	GLuint Score;					//游戏分数
	GLboolean KeysProcessed[1024];	//表示处理过的按键
	//构造/析构函数
	Game(GLuint width, GLuint height);
	~Game();
	//游戏初始化（加载所有的着色器/纹理/关卡）
	void Init();
	//游戏循环
	void ProcessInput(GLfloat dt);	//处理输入
	void Update(GLfloat dt);		//更新游戏设置状态
	void Render();					//渲染
	void DoCollisions();			//碰撞检测,设置此函数便于组织碰撞检测代码
	//重置
	void ResetLevel();
	void ResetPlayer();
	//管理道具
	//void SpawnPowerUps(GameObject &block);	//在给定砖块位置生成道具
	//void UpdatePowerUps(GLfloat dt);		//管理当前所有被激活的道具
};

#endif
