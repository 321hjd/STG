/*******************************************************************
** This code is part of STG
** 功能：管理特效粒子生成
** 使用库：头文件ParticleGenerator
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/


#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
	:shader(shader), texture(texture), amount(amount)
{
	this->init();
}

void ParticleGenerator::Update(GLfloat dt, GameObject & object, GLuint newParticles, glm::vec2 offset)
{
	//增加新的粒子
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}
	//更新所有的粒子
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.Life -= dt;				//随时间减少生命
		if (p.Life > 0.0f)			//如果粒子还活着，就更新属性
		{
			p.Position -= p.Velocity * dt;
			p.Color -= dt * 2.5;
		}
	}
}

//渲染所有的粒子
void ParticleGenerator::Draw()
{
	//使用添加剂混合给它一个“发光”的效果
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.Use();
	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			this->shader.SetVector2f("offset", particle.Position);
			this->shader.SetVector4f("color", particle.Color);
			this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	//别忘了重置为默认混合模式
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	//设置网格和属性
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	//填充网格缓冲
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	//设置网格属性
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	//创建粒子实例
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

//存贮上一个使用过的粒子下标，用于找到第一个死亡的粒子
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	//从上一个使用过的粒子开始检索
	for (GLuint i = lastUsedParticle; i < this->amount; ++i)
	{
		if (this->particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}
	//否则，做线性查找
	for (GLuint i = 0; i < lastUsedParticle; ++i)
	{
		if (particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}
	//如果没有死亡粒子，则返回索引0，即覆盖第一个粒子
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle & particle, GameObject & object, glm::vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}
