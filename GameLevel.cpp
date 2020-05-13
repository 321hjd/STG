/*******************************************************************
** This code is part of STG
** 功能：管理和加载关卡数据
** 使用库：系统库fstream、sstream，头文件GameLevel
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#include "GameLevel.h"

#include<fstream>
#include<sstream>


void GameLevel::Load(const GLchar * file, GLuint levelWidth, GLuint levelHeight)
{
	//加载下一关前清除所有的数据
	this->Planes.clear();
	//从文件加载数据
	GLuint tileCode;
	GameLevel level;							//似乎是多余的??
	std::string line;
	std::ifstream fstream(file);				//文件对象
	std::vector<std::vector<GLuint>> tileData;	//文件数据
	if (fstream)
	{
		while (std::getline(fstream, line))		//一行一行读
		{
			std::istringstream sstream(line);	//从line这个string对象中读取字符串
			std::vector<GLuint> row;
			while (sstream >> tileCode)			//读取并写入tileCode
				row.push_back(tileCode);
			tileData.push_back(row);			//加入一行砖块的数据
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer & renderer)
{
	for (PlaneObject &plane : this->Planes)								//遍历Planes数组
	{
		if (!plane.Destroyed && plane.Position.y + plane.Size.y > 0)	//只有当飞机进入屏幕内且未被摧毁时才渲染
			plane.Draw(renderer);
		if (plane.Position.y + plane.Size.y > 0)						//即便飞机被摧毁，子弹仍然渲染
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
		if (!plane.Destroyed && plane.Position.y + plane.Size.y > 0 && plane.Stayed)		//只有当飞机没被摧毁且进入屏幕以内才压入子弹
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
	//计算每个维度大小
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	//飞机大小
	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);		//飞机宽度
	GLfloat unit_height = 50;	//飞机高度
	//基于tileData初始化关卡数据
	for (GLuint y = 0; y < height; ++y)			//for循环中的++i和i++没有区别
	{
		for (GLuint x = 0; x < width; ++x)
		{
			//检查飞机类型
			if (tileData[y][x] == 1)		//敌机boss
			{
				glm::vec2 size(unit_width * 3, unit_height * 3);
				glm::vec2 pos(unit_width * x, -size.y * y);
				GLuint type = 2;			//对应枚举变量BOSS的值
				PlaneObject obj(pos, size, ResourceManager::GetTexture("plane_boss"), type);
				obj.Rotation = 135.1f;
				this->Planes.push_back(obj);
			}
			else if (tileData[y][x] == 2)	//普通敌机
			{
				glm::vec2 size(unit_width, unit_height);
				glm::vec2 pos(unit_width * x, -size.y * 2 * y);
				GLuint type = 0;			//对应枚举变量COMMON的值
				PlaneObject obj(pos, size, ResourceManager::GetTexture("plane_common"), type);
				obj.Rotation = 135.1f;
				this->Planes.push_back(obj);
			}
			else if (tileData[y][x] == 3)	//强化版敌机
			{
				glm::vec2 size(unit_width * 1.5, unit_height * 1.5);
				glm::vec2 pos(unit_width * x, -size.y * 2 * y);
				GLuint type = 1;			//对应枚举变量INTENSIFY的值
				PlaneObject obj(pos, size, ResourceManager::GetTexture("plane_intensify"), type);
				obj.Rotation = 135.1f;
				this->Planes.push_back(obj);
			}
		}
	}
}