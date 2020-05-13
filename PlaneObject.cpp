#include "PlaneObject.h"

PlaneObject::PlaneObject()
	: GameObject(), Type(COMMON), Blood(1), Track(P_STRAIGHT), Activated(GL_FALSE), Stayed(GL_FALSE), Timer(5000){}

PlaneObject::PlaneObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, GLuint type)
	:GameObject(pos, size, sprite, glm::vec3(1.0f), glm::vec2(0.0f, 50.0f)), Type(type), Blood(1), Track(P_STRAIGHT), Activated(GL_FALSE), Stayed(GL_FALSE), Timer(5000)
{
	switch (type)
	{
	case COMMON:
	{
		Blood = 5;
		Track = P_STRAIGHT;
		Score = 1000;
		break;
	}
	case INTENSIFY:
	{
		Blood = 10;
		Track = P_ARC;
		Score = 5000;
		break;
	}
	case BOSS:
	{
		Blood = 50;
		Track = P_STRAIGHT;
		Score = 20000;
		break;
	}
	default:
		break;
	}

}

//void PlaneObject::Check(GLuint window_height)
//{
//	if (this->Position.y >= window_height)
//		this->Destroyed = GL_TRUE;
//}

glm::vec2 PlaneObject::Move(GLfloat dt, GLuint window_height)
{
	switch (this->Track)
	{
	case P_STILL:
	{
		this->Position = this->Position;
		break;
	}
	case P_STRAIGHT:
	{
		this->Position += this->Velocity * dt;
		break;
	}
	case P_ARC:
	{
		this->Position += this->Velocity * sin(dt);
		break;
	}
	case P_SECTOR:
	{

		break;
	}
	default:
	{
		this->Position = this->Position;
		break;
	}
	}
	if (this->Position.y >= window_height)
		this->Destroyed = GL_TRUE;
	return this->Position;
}
//
//void PlaneObject::Stay_still(GLuint time)  
//{
//	GLuint t = time;
//	this->Track = P_STILL;
//	while (t)
//	{
//		t--;
//	};
//	this->Track = P_STRAIGHT;
//}

