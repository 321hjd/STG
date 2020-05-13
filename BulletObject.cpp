#define BOBJ
#ifdef BOBJ

#include "BulletObject.h"

BulletObject::BulletObject()
	:GameObject(), Type(SINGLE), Direction(-1),Track(STRAIGHT), Damage(1) {}

BulletObject::BulletObject(glm::vec2 pos, Texture2D sprite, GLuint type, GLfloat direction = -1)
	:GameObject(pos,glm::vec2(10.0f,20.0f),sprite,glm::vec3(1.0f),glm::vec2(0.0f,200.0f)),Type(SINGLE),Direction(direction),Track(STRAIGHT),Damage(1) 
{
	switch (type)
	{
	case SINGLE:
	{
		Track = STRAIGHT;
		break;
	}
	case DOUBLE:
	{
		Track = ARC;
		break;
	}
	default:
		break;
	}
}

//void BulletObject::Check(GLuint window_width, GLuint window_height)
//{
//	if (this->Position.x <= 0.0f || this->Position.y <= 0.0f || this->Position.x + this->Size.x >= window_width || this->Position.y + this->Size.y >= window_height)
//	{
//		this->Destroyed = GL_TRUE;
//	}
//}

glm::vec2 BulletObject::Move(GLfloat dt, GLuint window_width, GLuint window_height)
{
	if (!Destroyed)
	{
		switch (this->Track)
		{
		case STRAIGHT:
		{
			this->Position += this->Velocity * dt * this->Direction;
			break;
		}
		case ARC:
		{
			this->Position += this->Velocity * this->Direction * sin(dt);
			break;
		}
		case SECTOR:
		{

			break;
		}
		default:
		{

			break;
		}
		}
	}
	if (this->Position.x <= 0.0f || this->Position.y <= 0.0f || this->Position.x + this->Size.x >= window_width || this->Position.y + this->Size.y >= window_height)
		this->Destroyed = GL_TRUE;
	return this->Position;
}

#endif