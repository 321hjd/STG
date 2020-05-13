/*****************************************************************
** This code is part of STG
** 功能：游戏框架，包含所有相关的渲染和游戏代码，简单管理游戏代码，同时将所有的窗口代码从游戏中解耦
** 使用库：系统库algorithm、sstream，工具库irrKlang，头文件Game、SpriteRenderer、ResourceManager、
			GameObject、BallObject、ParticleGenerator、PostProcessor、TextRenderer
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#define GLFW_INCLUDE_NONE

#include<algorithm>			//使用其中的remove_if函数
#include<sstream>

#include"includes/irrKlang/irrKlang.h"
using namespace irrklang;


#include"Game.h"
#include"SpriteRenderer.h"
#include"ResourceManager.h"
#include"GameObject.h"
#include"PlaneObject.h"
#include"BulletObject.h"
#include"BulletLink.h"
#include"ParticleGenerator.h"
#include"PostProcessor.h"
#include"TextRenderer.h"


//游戏相关对象
SpriteRenderer       *Renderer;
PlaneObject          *Player;
//ParticleGenerator    *Particles;
PostProcessor        *Effects;
ISoundEngine         *SoundEngine = createIrrKlangDevice();
TextRenderer         *Text;
GLfloat              ShakeTime = 0.0f;

Game::Game(GLuint width, GLuint height) :
	State(GAME_MENU), Keys(), Width(width), Height(height), Level(0), Lives(3) ,Score(0){}

Game::~Game()
{
	delete Renderer;
	delete Player;
	//delete Particles;
	delete Effects;
	delete Text;
	SoundEngine->drop();
}

void Game::Init()
{
	//加载着色器
	ResourceManager::LoadShader("./shader/sprite.vert", "./shader/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("./shader/particle.vert", "./shader/particle.frag", nullptr, "particle");
	ResourceManager::LoadShader("./shader/post_processing.vert", "./shader/post_processing.frag", nullptr, "postprocessing");
	//配置着色器
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);	//设置正射投影矩阵
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);//着色器对象对应名称‘sprite’，将uniform采样对象image设为0
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
	//加载纹理
	ResourceManager::LoadTexture("./resources/textures/background.jpg", GL_FALSE, "background");//加载并将此纹理命名为background
	ResourceManager::LoadTexture("./resources/textures/plane_boss1.png", GL_TRUE, "plane_boss");
	ResourceManager::LoadTexture("./resources/textures/plane_common1.png", GL_TRUE, "plane_common");
	ResourceManager::LoadTexture("./resources/textures/plane_intensify1.png", GL_TRUE, "plane_intensify");
	ResourceManager::LoadTexture("./resources/textures/self_plane1.png", GL_TRUE, "self_plane");
	ResourceManager::LoadTexture("./resources/textures/bullet.png", GL_FALSE, "bullet");
	//ResourceManager::LoadTexture("./resources/textures/particle.png", GL_TRUE, "particle");
	ResourceManager::LoadTexture("./resources/textures/powerup_speed.png", GL_TRUE, "powerup_speed");
	ResourceManager::LoadTexture("./resources/textures/powerup_sticky.png", GL_TRUE, "powerup_sticky");
	ResourceManager::LoadTexture("./resources/textures/powerup_increase.png", GL_TRUE, "powerup_increase");
	ResourceManager::LoadTexture("./resources/textures/powerup_confuse.png", GL_TRUE, "powerup_confuse");
	ResourceManager::LoadTexture("./resources/textures/powerup_chaos.png", GL_TRUE, "powerup_chaos");
	ResourceManager::LoadTexture("./resources/textures/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");
	//设置渲染
	Shader shader_temp = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(shader_temp);
	//Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("./resources/fonts/arial.ttf", 24);
	//加载关卡
	GameLevel one; one.Load("./resources/checkpoints/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("./resources/checkpoints/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("./resources/checkpoints/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("./resources/checkpoints/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
	//配置游戏对象
	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	//glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Player = new PlaneObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("self_plane"),0);
	//Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
	//加载音乐
	SoundEngine->play2D("./resources/music/breakout.mp3", GL_TRUE);//文件路径+是否循环播放
}

void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1) % 4;
			this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
		{
			if (this->Level > 0)
				--this->Level;
			else
				this->Level = 3;
			this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
		}
	}
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		//移动飞机
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_W])
		{
			if (Player->Position.y >= 0)
			{
				Player->Position.y -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_S])
		{
			if (Player->Position.y <= this->Height - Player->Size.y)
			{
				Player->Position.y += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_MENU;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
	}
	if (this->State == GAME_WIN || this->State == GAME_LOSE)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			Effects->Chaos = GL_FALSE;
			Effects->Confuse = GL_FALSE;
			this->State = GAME_MENU;
		}
	}
}

void Game::Update(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		this->Levels[this->Level].Move(dt,this->Height,this->Width);
		if (Player->Bullet_link.Timer(dt))
			Player->Bullet_link.Add_bullet(glm::vec2(Player->Position.x + Player->Size.x / 2 - 5.0f, Player->Position.y), ResourceManager::GetTexture("bullet"), 0, -1);
		if (Player->Bullet_link.Timer(0.15*dt))
			this->Levels[this->Level].Add_bullet();
		Player->Bullet_link.Move_all(dt, this->Width, this->Height);
	}
	//检测碰撞
	this->DoCollisions();

	////更新粒子
	//Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
	////更新道具状态
	//this->UpdatePowerUps(dt);
	////减少shake的时间
	//if (ShakeTime > 0.0f)
	//{
	//	ShakeTime -= dt;
	//	if (ShakeTime <= -0.0f)
	//		Effects->Shake = GL_FALSE;
	//}
	////检查剩下的情况
	//if (Ball->Position.y >= this->Height)//当球到达底部
	//{
	//	--this->Lives;
	//	//玩家是否已失去所有生命值？：游戏结束
	//	if (this->Lives == 0)
	//	{
	//		this->PowerUps.clear();		//重置时也清除道具
	//		this->ResetLevel();
	//		this->State = GAME_MENU;
	//	}
	//	this->ResetPlayer();
	//}
	if (this->State == GAME_ACTIVE && this->Lives == 0)
	{
		this->ResetLevel();
		this->ResetPlayer();
		Effects->Confuse = GL_TRUE;
		this->State = GAME_LOSE;
	}
	//获胜
	if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsComplete())
	{
		this->ResetLevel();
		this->ResetPlayer();
		Effects->Chaos = GL_TRUE;
		this->State = GAME_WIN;
	}
}

void Game::Render()
{
	//显示关卡
	std::stringstream s_level;
	s_level << this->Level + 1;

	//渲染游戏
	if (this->State == GAME_ACTIVE || this->State == GAME_WIN || this->State == GAME_LOSE)
	{
		//开始渲染后期处理四边形
		Effects->BeginRender();
		//渲染背景
		Texture2D texture_temp = ResourceManager::GetTexture("background");
		Renderer->DrawSprite(texture_temp, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		//渲染关卡
		this->Levels[this->Level].Draw(*Renderer);
		//渲染玩家
		Player->Draw(*Renderer);
		Player->Bullet_link.Draw_all(*Renderer);
		//渲染粒子
		//Particles->Draw();
		//渲染道具
		//for (PowerUp &powerUp : this->PowerUps)
		//	if (!powerUp.Destroyed)
		//		powerUp.Draw(*Renderer);
		//后期处理特效
		Effects->EndRender();
		Effects->Render(glfwGetTime());
		//文本渲染
		std::stringstream s_live, s_score;
		s_live << this->Lives;
		s_score << this->Score;
		Text->RenderText("Lives:" + s_live.str(), 5.0f, 5.0f, 1.0f);
		Text->RenderText("Score:" + s_score.str(), 5.0f, 65.0f, 1.0f);
		Text->RenderText("Level:" + s_level.str(), 5.0f, 35.0f, 1.0f);
	}
	//菜单状态
	if (this->State == GAME_MENU)
	{
		Texture2D texture_temp = ResourceManager::GetTexture("background");
		Renderer->DrawSprite(texture_temp, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 0.75f);
		Text->RenderText("Level:" + s_level.str(), 5.0f, 30.0f, 1.0f);
	}
}

void Game::ResetLevel()
{
	if (this->Level == 0)
		this->Levels[0].Load("./resources/checkpoints/one.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 1)
		this->Levels[1].Load("./resources/checkpoints/two.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 2)
		this->Levels[2].Load("./resources/checkpoints/three.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 3)
		this->Levels[3].Load("./resources/checkpoints/four.lvl", this->Width, this->Height * 0.5f);
	this->Lives = 3;		//重置关卡时将玩家生命值置为3
	this->Score = 0;		//重置关卡时将玩家f分数置为3
}

void Game::ResetPlayer()
{
	//重置玩家的状态
	Player->Destroyed = GL_FALSE;
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player->Bullet_link.Clear_all();
	//Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
	//重置道具效果
	//Effects->Chaos = Effects->Confuse = GL_FALSE;
	//Ball->PassThrough = Ball->Sticky = GL_FALSE;
	//Player->Color = glm::vec3(1.0f);
	//Ball->Color = glm::vec3(1.0f);
}

////道具处理-----------------------------------------------------------------
//
////随机生成道具
////GLboolean ShouldSpawn(GLuint chance)
////{
////	GLuint random = rand() % chance;
////	return random == 0;
////}
////
//////激活道具,更改部分游戏对象的属性
////void ActivatePowerUp(PowerUp &powerUp)
////{
////	//根据道具类型激活道具
////	if (powerUp.Type == "speed")
////	{
////		Ball->Velocity *= 1.2;
////	}
////	else if (powerUp.Type == "sticky")
////	{
////		Ball->Sticky = GL_TRUE;
////		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
////	}
////	else if (powerUp.Type == "pass-through")
////	{
////		Ball->PassThrough = GL_TRUE;
////		Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
////	}
////	else if (powerUp.Type == "pad-size-increase")
////	{
////		Player->Size.x += 50;
////	}
////	else if (powerUp.Type == "confuse")
////	{
////		if (!Effects->Chaos)
////			Effects->Confuse = GL_TRUE;//只有在chaos未激活时生效，chaos同理
////	}
////	else if (powerUp.Type == "chaos")
////	{
////		if (!Effects->Confuse)
////			Effects->Chaos = GL_TRUE;
////	}
////}
////
//////检测是否有同类道具生效
////GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
////{
////	for (const PowerUp &powerUp : powerUps)
////	{
////		if (powerUp.Activated)
////			if (powerUp.Type == type)
////				return GL_TRUE;
////	}
////	return GL_FALSE;
////}
////
////void Game::SpawnPowerUps(GameObject & block)
////{
////	if (ShouldSpawn(75))  //1/75的几率
////		this->PowerUps.push_back(
////			PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_speed"))
////		);						//若duration的值为0.0f则表示它持续无限长的时间
////	if (ShouldSpawn(75))
////		this->PowerUps.push_back(
////			PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_sticky"))
////		);
////	if (ShouldSpawn(75))
////		this->PowerUps.push_back(
////			PowerUp("pass-through", glm::vec3(1.0f, 0.6f, 0.4f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough"))
////		);
////	if (ShouldSpawn(75))
////		this->PowerUps.push_back(
////			PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 15.0f, block.Position, ResourceManager::GetTexture("powerup_increase"))
////		);
////	if (ShouldSpawn(15))		//负面道具被更频繁地生成
////		this->PowerUps.push_back(
////			PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse"))
////		);
////	if (ShouldSpawn(15))
////		this->PowerUps.push_back(
////			PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos"))
////		);
////}
//
////根据道具的速度移动它，并减少已激活道具的持续时间，
////每当时间减少至小于0时，我们令其失效，并恢复相关变量的状态。
///*---------------------------------------------------------------
//当一个道具在激活状态时，另一个道具与挡板发生了接触。在这种情况下我们有
//超过1个在当前PowerUps容器中处于激活状态的道具。然后，当这些道具中的一个被
//停用时，我们不应使其效果失效因为另一个相同类型的道具仍处于激活状态。出于这
//个原因，我们使用isOtherPowerUpActive检查是否有同类道具处于激活状态。只有当
//它返回false时，我们才停用这个道具的效果。这样，给定类型的道具的持续时间就
//可以延长至最近一次被激活后的持续时间
//-------------------------------------------------------------------*/
////void Game::UpdatePowerUps(GLfloat dt)
////{
////	for (PowerUp &powerUp : this->PowerUps)
////	{
////		powerUp.Position += powerUp.Velocity * dt;
////		if (powerUp.Activated)
////		{
////			powerUp.Duration -= dt;
////			if (powerUp.Duration <= 0.0f)
////			{
////				//时间小于0之后将这个道具移除
////				powerUp.Activated = GL_FALSE;
////				//停用效果
////				if (powerUp.Type == "sticky")
////				{
////					if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
////					{
////						//仅当没有其它sticky效果处于激活状态时重置，以下同理
////						Ball->Sticky = GL_FALSE;
////						Player->Color = glm::vec3(1.0f);
////					}
////				}
////				else if (powerUp.Type == "pass-through")
////				{
////					if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
////					{
////						Ball->PassThrough = GL_FALSE;
////						Ball->Color = glm::vec3(1.0f);
////					}
////				}
////				else if (powerUp.Type == "confuse")
////				{
////					if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
////					{
////						Effects->Confuse = GL_FALSE;
////					}
////				}
////				else if (powerUp.Type == "chaos")
////				{
////					if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
////					{
////						Effects->Chaos = GL_FALSE;
////					}
////				}
////			}
////		}
////	}
////	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
////		[](const PowerUp &powerUp) {return powerUp.Destroyed && !powerUp.Activated; }
////	), this->PowerUps.end());
////}
///*-----------------------------------------------
//remove_if函数将lamda表达式为true的元素移动至容器的
//末尾并返回一个迭代器指向应被移除的元素范围的开始部
//分。容器的erase函数接着擦除这个迭代器指向的元素与容
//器末尾元素之间的所有元素
//------------------------------------------------*/
//
////-------------------------------------------------------------------------
//
////碰撞检测
////GLboolean CheckCollision(GameObject &one, GameObject &two);//AABB-AABB
////Collision CheckCollision(BallObject &one, GameObject &two);//AABB-Circle
//////撞击方向判断
////Direction VectorDirection(glm::vec2 closest);
////
////void Game::DoCollisions()
////{
////	for (GameObject &box : this->Levels[this->Level].Planes)
////	{
////		if (!box.Destroyed)
////		{
////			Collision collision = CheckCollision(*Ball, box);
////			if (std::get<0>(collision))//用get<常量>访问元组tuple,0代表是第一个GLboolean
////			{
////				//如果砖块不是实心
////				if (!box.IsSolid)
////				{
////					box.Destroyed = GL_TRUE;
////					this->SpawnPowerUps(box);
////					SoundEngine->play2D("./resources/music/bleep.mp3", GL_FALSE);
////				}
////				else
////				{
////					//如果撞到实心砖块，实现晃动效果
////					ShakeTime = 0.05f;
////					Effects->Shake = GL_TRUE;
////					SoundEngine->play2D("./resources/music/solid.wav", GL_FALSE);
////				}
////				//碰撞处理
////				Direction dir = std::get<1>(collision);
////				glm::vec2 diff_vector = std::get<2>(collision);
////				//当PassThrough激活，遇到非实心砖块不进行碰撞处理
////				if (!(Ball->PassThrough && !box.IsSolid))
////				{
////					if (dir == LEFT || dir == RIGHT)//水平方向碰撞
////					{
////						Ball->Velocity.x = -Ball->Velocity.x;//反转水平速度
////						//重定位
////						GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);//球侵入方块的长度
////						if (dir == LEFT)
////							Ball->Position.x += penetration;//将球右移
////						else
////							Ball->Position.x -= penetration;//将球左移
////					}
////					else //垂直方向碰撞
////					{
////						Ball->Velocity.y = -Ball->Velocity.y;//反转垂直速度
////						//重定位
////						GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
////						if (dir == UP)
////							Ball->Position.y -= penetration;//将球上移
////						else
////							Ball->Position.y += penetration;//将球下移
////					}
////				}
////			}
////		}
////	}
////	//检测砖块碰撞的同时检测和道具的接触
////	for (PowerUp &powerUp : this->PowerUps)
////	{
////		if (!powerUp.Destroyed)
////		{
////			if (powerUp.Position.y >= this->Height)//道具掉出屏幕，销毁
////				powerUp.Destroyed = GL_TRUE;
////			if (CheckCollision(*Player, powerUp))
////			{
////				//道具与挡板接触，激活
////				ActivatePowerUp(powerUp);
////				powerUp.Destroyed = GL_TRUE;
////				powerUp.Activated = GL_TRUE;
////				SoundEngine->play2D("./resources/music/powerup.wav");
////			}
////		}
////	}
////	//同时检查板子的碰撞
////	Collision result = CheckCollision(*Ball, *Player);
////	if (!Ball->Stuck && std::get<0>(result))
////	{
////		//检查球碰撞在板子的左侧还是右侧,并据此改变球的速度方向
////		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
////		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
////		GLfloat percentage = distance / (Player->Size.x / 2);
////		//根据结果移动
////		GLfloat strength = 2.0f;
////		glm::vec2 oldVelocity = Ball->Velocity;
////		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;//距离板子中心越远，速度反弹越大
////		//反转球的y方向速度
////		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
////		Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
////
////		//将球的stuck属性设置为自己的sticky属性，sticky被激活则stuck为真，球粘滞
////		Ball->Stuck = Ball->Sticky;
////
////		SoundEngine->play2D("./resources/music/bleep.wav");
////	}
////}
////
////
////
//////AABB-AABB
////GLboolean CheckCollision(GameObject &one, GameObject &two)
////{
////	//x轴方向碰撞？
////	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
////		two.Position.x + two.Size.x >= one.Position.x;
////	// y轴方向碰撞？
////	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
////		two.Position.y + two.Size.y >= one.Position.y;
////	// 只有两个轴向都有碰撞时才碰撞
////	return collisionX && collisionY;
////}
////
//////碰撞检测（AABB-Circle collision）
////Collision CheckCollision(BallObject &one, GameObject &two)
////{
////	// 获取圆的中心 
////	glm::vec2 center(one.Position + one.Radius);
////	// 计算AABB的信息（中心、半边长）
////	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
////	glm::vec2 aabb_center(
////		two.Position.x + aabb_half_extents.x,
////		two.Position.y + aabb_half_extents.y
////	);
////	// 获取两个中心的差矢量
////	glm::vec2 difference = center - aabb_center;
////	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
////	// AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
////	glm::vec2 closest = aabb_center + clamped;
////	// 获得圆心center和最近点closest的矢量并判断是否 length <= radius
////	difference = closest - center;
////	if (glm::length(difference) <= one.Radius)
////		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
////	else
////		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));//make_tuple用于初始化tuple容器
////}
////
//////撞击方向判断
////Direction VectorDirection(glm::vec2 target)
////{
////	glm::vec2 compass[] = {
////		glm::vec2(0.0f,1.0f),	//上
////		glm::vec2(1.0f,0.0f),	//右
////		glm::vec2(0.0f,-1.0f),	//下
////		glm::vec2(-1.0f,0.0f)	//左
////	};
////	GLfloat max = 0.0f;
////	GLuint best_match = -1;
////	for (GLuint i = 0; i < 4; i++)
////	{
////		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);//点积
////		if (dot_product > max)		//取点积最大的，即为碰撞点所在的一边
////		{
////			max = dot_product;
////			best_match = i;
////		}
////	}
////	return (Direction)best_match;
////}
//
////void Game::DoCollisions()
////{
////	for (GameObject &box : this->Levels[this->Level].Planes)
////	{
////		if (!box.Destroyed)
////		{
////			Collision collision = CheckCollision(*Ball, box);
////			if (std::get<0>(collision))//用get<常量>访问元组tuple,0代表是第一个GLboolean
////			{
////				//如果砖块不是实心
////				if (!box.IsSolid)
////				{
////					box.Destroyed = GL_TRUE;
////					this->SpawnPowerUps(box);
////					SoundEngine->play2D("./resources/music/bleep.mp3", GL_FALSE);
////				}
////				else
////				{
////					//如果撞到实心砖块，实现晃动效果
////					ShakeTime = 0.05f;
////					Effects->Shake = GL_TRUE;
////					SoundEngine->play2D("./resources/music/solid.wav", GL_FALSE);
////				}
////				//碰撞处理
////				Direction dir = std::get<1>(collision);
////				glm::vec2 diff_vector = std::get<2>(collision);
////				//当PassThrough激活，遇到非实心砖块不进行碰撞处理
////				if (!(Ball->PassThrough && !box.IsSolid))
////				{
////					if (dir == LEFT || dir == RIGHT)//水平方向碰撞
////					{
////						Ball->Velocity.x = -Ball->Velocity.x;//反转水平速度
////						//重定位
////						GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);//球侵入方块的长度
////						if (dir == LEFT)
////							Ball->Position.x += penetration;//将球右移
////						else
////							Ball->Position.x -= penetration;//将球左移
////					}
////					else //垂直方向碰撞
////					{
////						Ball->Velocity.y = -Ball->Velocity.y;//反转垂直速度
////						//重定位
////						GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
////						if (dir == UP)
////							Ball->Position.y -= penetration;//将球上移
////						else
////							Ball->Position.y += penetration;//将球下移
////					}
////				}
////			}
////		}
////	}
////	//检测砖块碰撞的同时检测和道具的接触
////	for (PowerUp &powerUp : this->PowerUps)
////	{
////		if (!powerUp.Destroyed)
////		{
////			if (powerUp.Position.y >= this->Height)//道具掉出屏幕，销毁
////				powerUp.Destroyed = GL_TRUE;
////			if (CheckCollision(*Player, powerUp))
////			{
////				//道具与挡板接触，激活
////				ActivatePowerUp(powerUp);
////				powerUp.Destroyed = GL_TRUE;
////				powerUp.Activated = GL_TRUE;
////				SoundEngine->play2D("./resources/music/powerup.wav");
////			}
////		}
////	}
////	//同时检查板子的碰撞
////	Collision result = CheckCollision(*Ball, *Player);
////	if (!Ball->Stuck && std::get<0>(result))
////	{
////		//检查球碰撞在板子的左侧还是右侧,并据此改变球的速度方向
////		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
////		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
////		GLfloat percentage = distance / (Player->Size.x / 2);
////		//根据结果移动
////		GLfloat strength = 2.0f;
////		glm::vec2 oldVelocity = Ball->Velocity;
////		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;//距离板子中心越远，速度反弹越大
////		//反转球的y方向速度
////		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
////		Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
////
////		//将球的stuck属性设置为自己的sticky属性，sticky被激活则stuck为真，球粘滞
////		Ball->Stuck = Ball->Sticky;
////
////		SoundEngine->play2D("./resources/music/bleep.wav");
////	}
////}

//碰撞检测
//飞机和子弹
GLboolean CheckCollision(PlaneObject &one, BulletObject &two)
{
		//x轴方向碰撞
		bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
			two.Position.x + two.Size.x >= one.Position.x;
		// y轴方向碰撞？
		bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
			two.Position.y + two.Size.y >= one.Position.y;
		// 只有两个轴向都有碰撞时才碰撞
		return collisionX && collisionY;
}
//飞机和飞机
GLboolean CheckCollision(PlaneObject &one, PlaneObject &two)
{
	//x轴方向碰撞
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// y轴方向碰撞？
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// 只有两个轴向都有碰撞时才碰撞
	return collisionX && collisionY;
}

void Game::DoCollisions()
{
	for (PlaneObject &plane : this->Levels[this->Level].Planes)
	{
		if (!plane.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, plane);	//检测自机和敌机碰撞
			if (collision)
			{
				plane.Blood--;
				Player->Destroyed = GL_TRUE;
				SoundEngine->play2D("./resources/music/bleep.mp3", GL_FALSE);
			}
			for (BulletObject &bullet : plane.Bullet_link.Bullets)
			{
				if (!bullet.Destroyed)
				{
					GLboolean collision = CheckCollision(*Player, bullet);	//检测自机和子弹碰撞
					if (collision)
					{
						bullet.Destroyed = GL_TRUE;
						Player->Destroyed = GL_TRUE;			
						SoundEngine->play2D("./resources/music/powerup.wav", GL_FALSE);
					}
				}
			}
			for (BulletObject &bullet : Player->Bullet_link.Bullets)
			{
				if (!bullet.Destroyed)
				{
					GLboolean collision = CheckCollision(plane, bullet);	//检测自机和子弹碰撞
					if (collision)
					{
						plane.Blood -= bullet.Damage;
						bullet.Destroyed = GL_TRUE;
						if (plane.Blood == 0)
						{
							plane.Destroyed = GL_TRUE;
							this->Score += plane.Score;
						}
						SoundEngine->play2D("./resources/music/bleep.mp3", GL_FALSE);
					}
				}
			}
		}
	}
	if (Player->Destroyed) {
		this->Lives--;
		this->ResetPlayer();
	}
}