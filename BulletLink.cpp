#define Bul_Link
#ifdef Bul_Link

#include "BulletLink.h"

BulletLink::BulletLink()
{
	fire_interval = 0;
}

void BulletLink::Move_all(GLfloat dt, GLuint window_width, GLuint window_height)
{
	for (BulletObject &bullet : Bullets)
		bullet.Move(dt, window_width, window_height);
}

void BulletLink::Draw_all(SpriteRenderer &renderer)
{
	for (BulletObject &bullet : Bullets)
		if (!bullet.Destroyed)
			bullet.Draw(renderer);
}

void BulletLink::Clear_all()
{
	this->Bullets.clear();
}

void BulletLink::Pop_bullet()
{
	if (Bullets.back().Destroyed)
		Bullets.pop_back();							//？？？？为什么是直接弹出最后一个而不是遍历Destroyed为true的？
}

void BulletLink::Add_bullet(glm::vec2 pos, Texture2D sprite, GLuint type, GLfloat direction)
{
	BulletObject newBullet(pos, sprite, type, direction);
	this->Bullets.push_back(newBullet);			//在链表开头插入一个子弹
}

GLboolean BulletLink::Timer(GLfloat dt)
{
	fire_interval += dt;						//射击的计时器，每当间隔到达5倍dt，返回true并将fire_interval重置为0
	if (fire_interval >= FIRE_INTERVAL)
	{
		fire_interval = 0;
		return GL_TRUE;
	}
	return GL_FALSE;
}

#endif