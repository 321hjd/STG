/*******************************************************************
** This code is part of STG
** 功能：管理基本游戏对象属性
** 使用库：头文件GameObject
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#include "GameObject.h"

GameObject::GameObject()
	:Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), Destroyed(false) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
	: Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), Destroyed(false) {}

void GameObject::Draw(SpriteRenderer & renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
