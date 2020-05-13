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
		Bullets.pop_back();							//��������Ϊʲô��ֱ�ӵ������һ�������Ǳ���DestroyedΪtrue�ģ�
}

void BulletLink::Add_bullet(glm::vec2 pos, Texture2D sprite, GLuint type, GLfloat direction)
{
	BulletObject newBullet(pos, sprite, type, direction);
	this->Bullets.push_back(newBullet);			//������ͷ����һ���ӵ�
}

GLboolean BulletLink::Timer(GLfloat dt)
{
	fire_interval += dt;						//����ļ�ʱ����ÿ���������5��dt������true����fire_interval����Ϊ0
	if (fire_interval >= FIRE_INTERVAL)
	{
		fire_interval = 0;
		return GL_TRUE;
	}
	return GL_FALSE;
}

#endif