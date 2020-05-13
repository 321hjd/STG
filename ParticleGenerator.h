#pragma once

/*******************************************************************
** This code is part of STG
** 功能：管理特效粒子生成
** 使用库：系统库vector，工具库glad、glm，头文件Shader、Texture、GameObject
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/


#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include "includes/glad.h"
#include "includes/glm/glm.hpp"

#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

//粒子属性和状态
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;
	//使用初始化列表初始化数据
	Particle() :Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

//粒子发生器类
class ParticleGenerator
{
public:
	//构造函数
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	
	/*--------------------------------------------
	功能：更新所有粒子
	参数1：每一帧的时间
	参数2：游戏对象
	参数3：新增加粒子的数量
	参数4：粒子的偏移量
	输出：无
	编写于 ？？？？？？？？？？？？？？
	---------------------------------------------*/
	void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	
	//渲染所有粒子
	void Draw();
private:
	//粒子状态
	std::vector<Particle> particles;	//粒子容器
	GLuint amount;						//粒子数量
	//渲染状态
	Shader shader;						//渲染粒子所使用的着色器
	Texture2D texture;					//渲染粒子所使用的纹理
	GLuint VAO;							//顶点数组对象
	
	void init();

	/*-------------------------------------
	功能：返回第一个死亡的粒子
	参数：无
	输出：第一个死亡粒子的位置索引
	编写于 ？？？？？？？？？？？？？？
	--------------------------------------*/
	GLuint firstUnusedParticle();
	
	/*------------------------------------
	功能：重生粒子
	参数1：粒子容器对象的引用
	参数2：游戏对象
	参数3：粒子偏移量
	输出：无
	编写于 ？？？？？？？？？？？？？？
	-------------------------------------*/
	void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};


#endif