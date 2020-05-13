/*******************************************************************
** This code is part of STG
** ���ܣ�����ͼ��عؿ�����
** ʹ�ÿ⣺ϵͳ��fstream��sstream��ͷ�ļ�GameLevel
** ��д�� ����������������������������
******************************************************************/

#include "GameLevel.h"

#include<fstream>
#include<sstream>


void GameLevel::Load(const GLchar * file, GLuint levelWidth, GLuint levelHeight)
{
	//������һ��ǰ������е�����
	this->Planes.clear();
	//���ļ���������
	GLuint tileCode;
	GameLevel level;							//�ƺ��Ƕ����??
	std::string line;
	std::ifstream fstream(file);				//�ļ�����
	std::vector<std::vector<GLuint>> tileData;	//�ļ�����
	if (fstream)
	{
		while (std::getline(fstream, line))		//һ��һ�ж�
		{
			std::istringstream sstream(line);	//��line���string�����ж�ȡ�ַ���
			std::vector<GLuint> row;
			while (sstream >> tileCode)			//��ȡ��д��tileCode
				row.push_back(tileCode);
			tileData.push_back(row);			//����һ��ש�������
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer & renderer)
{
	for (PlaneObject &plane : this->Planes)								//����Planes����
	{
		if (!plane.Destroyed && plane.Position.y + plane.Size.y > 0)	//ֻ�е��ɻ�������Ļ����δ���ݻ�ʱ����Ⱦ
			plane.Draw(renderer);
		if (plane.Position.y + plane.Size.y > 0)						//����ɻ����ݻ٣��ӵ���Ȼ��Ⱦ
			plane.Bullet_link.Draw_all(renderer);
	}
}

GLboolean GameLevel::IsComplete()
{
	for (PlaneObject &plane : this->Planes)
		if (!plane.Destroyed)
			return GL_FALSE;
	return GL_TRUE;
}

void GameLevel::Move(GLfloat dt, GLuint window_height, GLuint window_width)
{
	for (PlaneObject &plane : this->Planes)
	{
		if (!plane.Destroyed)
		{
			if (plane.Position.y >= window_height / 5)
			{
				if (plane.Type == BOSS)
				{
					plane.Track = P_STILL;
					plane.Stayed = GL_TRUE;
				}
				else if (plane.Track != P_STILL && plane.Stayed == GL_FALSE) {
					plane.Track = P_STILL;
					plane.Stayed = GL_TRUE;
				}
				else
				{
					if (plane.Timer-- == 0)
						plane.Track = P_STRAIGHT;
				}
			}
			plane.Move(dt, window_height);
		}
		if (plane.Position.y + plane.Size.y > 0)
			plane.Bullet_link.Move_all(dt, window_width, window_height);
	}
}

void GameLevel::Add_bullet()
{
	for (PlaneObject &plane : this->Planes)
	{
		if (!plane.Destroyed && plane.Position.y + plane.Size.y > 0 && plane.Stayed)		//ֻ�е��ɻ�û���ݻ��ҽ�����Ļ���ڲ�ѹ���ӵ�
			plane.Bullet_link.Add_bullet(glm::vec2(plane.Position.x + plane.Size.x / 2 - 5.0f, plane.Position.y + plane.Size.y), ResourceManager::GetTexture("bullet"), 0, 1);
	}
}

//void GameLevel::Check(GLuint window_height)
//{
//	for (PlaneObject &plane : this->Planes)
//		plane.Check(window_height);
//}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	//����ÿ��ά�ȴ�С
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	//�ɻ���С
	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);		//�ɻ����
	GLfloat unit_height = 50;	//�ɻ��߶�
	//����tileData��ʼ���ؿ�����
	for (GLuint y = 0; y < height; ++y)			//forѭ���е�++i��i++û������
	{
		for (GLuint x = 0; x < width; ++x)
		{
			//���ɻ�����
			if (tileData[y][x] == 1)		//�л�boss
			{
				glm::vec2 size(unit_width * 3, unit_height * 3);
				glm::vec2 pos(unit_width * x, -size.y * y);
				GLuint type = 2;			//��Ӧö�ٱ���BOSS��ֵ
				PlaneObject obj(pos, size, ResourceManager::GetTexture("plane_boss"), type);
				obj.Rotation = 135.1f;
				this->Planes.push_back(obj);
			}
			else if (tileData[y][x] == 2)	//��ͨ�л�
			{
				glm::vec2 size(unit_width, unit_height);
				glm::vec2 pos(unit_width * x, -size.y * 2 * y);
				GLuint type = 0;			//��Ӧö�ٱ���COMMON��ֵ
				PlaneObject obj(pos, size, ResourceManager::GetTexture("plane_common"), type);
				obj.Rotation = 135.1f;
				this->Planes.push_back(obj);
			}
			else if (tileData[y][x] == 3)	//ǿ����л�
			{
				glm::vec2 size(unit_width * 1.5, unit_height * 1.5);
				glm::vec2 pos(unit_width * x, -size.y * 2 * y);
				GLuint type = 1;			//��Ӧö�ٱ���INTENSIFY��ֵ
				PlaneObject obj(pos, size, ResourceManager::GetTexture("plane_intensify"), type);
				obj.Rotation = 135.1f;
				this->Planes.push_back(obj);
			}
		}
	}
}