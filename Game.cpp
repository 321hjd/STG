/*****************************************************************
** This code is part of STG
** ���ܣ���Ϸ��ܣ�����������ص���Ⱦ����Ϸ���룬�򵥹�����Ϸ���룬ͬʱ�����еĴ��ڴ������Ϸ�н���
** ʹ�ÿ⣺ϵͳ��algorithm��sstream�����߿�irrKlang��ͷ�ļ�Game��SpriteRenderer��ResourceManager��
			GameObject��BallObject��ParticleGenerator��PostProcessor��TextRenderer
** ��д�� ����������������������������
******************************************************************/

#define GLFW_INCLUDE_NONE

#include<algorithm>			//ʹ�����е�remove_if����
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


//��Ϸ��ض���
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
	//������ɫ��
	ResourceManager::LoadShader("./shader/sprite.vert", "./shader/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("./shader/particle.vert", "./shader/particle.frag", nullptr, "particle");
	ResourceManager::LoadShader("./shader/post_processing.vert", "./shader/post_processing.frag", nullptr, "postprocessing");
	//������ɫ��
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);	//��������ͶӰ����
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);//��ɫ�������Ӧ���ơ�sprite������uniform��������image��Ϊ0
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
	//��������
	ResourceManager::LoadTexture("./resources/textures/background.jpg", GL_FALSE, "background");//���ز�������������Ϊbackground
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
	//������Ⱦ
	Shader shader_temp = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(shader_temp);
	//Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("./resources/fonts/arial.ttf", 24);
	//���عؿ�
	GameLevel one; one.Load("./resources/checkpoints/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("./resources/checkpoints/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("./resources/checkpoints/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("./resources/checkpoints/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
	//������Ϸ����
	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	//glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Player = new PlaneObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("self_plane"),0);
	//Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
	//��������
	SoundEngine->play2D("./resources/music/breakout.mp3", GL_TRUE);//�ļ�·��+�Ƿ�ѭ������
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
		//�ƶ��ɻ�
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
	//�����ײ
	this->DoCollisions();

	////��������
	//Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
	////���µ���״̬
	//this->UpdatePowerUps(dt);
	////����shake��ʱ��
	//if (ShakeTime > 0.0f)
	//{
	//	ShakeTime -= dt;
	//	if (ShakeTime <= -0.0f)
	//		Effects->Shake = GL_FALSE;
	//}
	////���ʣ�µ����
	//if (Ball->Position.y >= this->Height)//���򵽴�ײ�
	//{
	//	--this->Lives;
	//	//����Ƿ���ʧȥ��������ֵ������Ϸ����
	//	if (this->Lives == 0)
	//	{
	//		this->PowerUps.clear();		//����ʱҲ�������
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
	//��ʤ
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
	//��ʾ�ؿ�
	std::stringstream s_level;
	s_level << this->Level + 1;

	//��Ⱦ��Ϸ
	if (this->State == GAME_ACTIVE || this->State == GAME_WIN || this->State == GAME_LOSE)
	{
		//��ʼ��Ⱦ���ڴ����ı���
		Effects->BeginRender();
		//��Ⱦ����
		Texture2D texture_temp = ResourceManager::GetTexture("background");
		Renderer->DrawSprite(texture_temp, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		//��Ⱦ�ؿ�
		this->Levels[this->Level].Draw(*Renderer);
		//��Ⱦ���
		Player->Draw(*Renderer);
		Player->Bullet_link.Draw_all(*Renderer);
		//��Ⱦ����
		//Particles->Draw();
		//��Ⱦ����
		//for (PowerUp &powerUp : this->PowerUps)
		//	if (!powerUp.Destroyed)
		//		powerUp.Draw(*Renderer);
		//���ڴ�����Ч
		Effects->EndRender();
		Effects->Render(glfwGetTime());
		//�ı���Ⱦ
		std::stringstream s_live, s_score;
		s_live << this->Lives;
		s_score << this->Score;
		Text->RenderText("Lives:" + s_live.str(), 5.0f, 5.0f, 1.0f);
		Text->RenderText("Score:" + s_score.str(), 5.0f, 65.0f, 1.0f);
		Text->RenderText("Level:" + s_level.str(), 5.0f, 35.0f, 1.0f);
	}
	//�˵�״̬
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
	this->Lives = 3;		//���ùؿ�ʱ���������ֵ��Ϊ3
	this->Score = 0;		//���ùؿ�ʱ�����f������Ϊ3
}

void Game::ResetPlayer()
{
	//������ҵ�״̬
	Player->Destroyed = GL_FALSE;
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player->Bullet_link.Clear_all();
	//Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
	//���õ���Ч��
	//Effects->Chaos = Effects->Confuse = GL_FALSE;
	//Ball->PassThrough = Ball->Sticky = GL_FALSE;
	//Player->Color = glm::vec3(1.0f);
	//Ball->Color = glm::vec3(1.0f);
}

////���ߴ���-----------------------------------------------------------------
//
////������ɵ���
////GLboolean ShouldSpawn(GLuint chance)
////{
////	GLuint random = rand() % chance;
////	return random == 0;
////}
////
//////�������,���Ĳ�����Ϸ���������
////void ActivatePowerUp(PowerUp &powerUp)
////{
////	//���ݵ������ͼ������
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
////			Effects->Confuse = GL_TRUE;//ֻ����chaosδ����ʱ��Ч��chaosͬ��
////	}
////	else if (powerUp.Type == "chaos")
////	{
////		if (!Effects->Confuse)
////			Effects->Chaos = GL_TRUE;
////	}
////}
////
//////����Ƿ���ͬ�������Ч
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
////	if (ShouldSpawn(75))  //1/75�ļ���
////		this->PowerUps.push_back(
////			PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_speed"))
////		);						//��duration��ֵΪ0.0f���ʾ���������޳���ʱ��
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
////	if (ShouldSpawn(15))		//������߱���Ƶ��������
////		this->PowerUps.push_back(
////			PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse"))
////		);
////	if (ShouldSpawn(15))
////		this->PowerUps.push_back(
////			PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos"))
////		);
////}
//
////���ݵ��ߵ��ٶ��ƶ������������Ѽ�����ߵĳ���ʱ�䣬
////ÿ��ʱ�������С��0ʱ����������ʧЧ�����ָ���ر�����״̬��
///*---------------------------------------------------------------
//��һ�������ڼ���״̬ʱ����һ�������뵲�巢���˽Ӵ��������������������
//����1���ڵ�ǰPowerUps�����д��ڼ���״̬�ĵ��ߡ�Ȼ�󣬵���Щ�����е�һ����
//ͣ��ʱ�����ǲ�Ӧʹ��Ч��ʧЧ��Ϊ��һ����ͬ���͵ĵ����Դ��ڼ���״̬��������
//��ԭ������ʹ��isOtherPowerUpActive����Ƿ���ͬ����ߴ��ڼ���״̬��ֻ�е�
//������falseʱ�����ǲ�ͣ��������ߵ�Ч�����������������͵ĵ��ߵĳ���ʱ���
//�����ӳ������һ�α������ĳ���ʱ��
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
////				//ʱ��С��0֮����������Ƴ�
////				powerUp.Activated = GL_FALSE;
////				//ͣ��Ч��
////				if (powerUp.Type == "sticky")
////				{
////					if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
////					{
////						//����û������stickyЧ�����ڼ���״̬ʱ���ã�����ͬ��
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
//remove_if������lamda���ʽΪtrue��Ԫ���ƶ���������
//ĩβ������һ��������ָ��Ӧ���Ƴ���Ԫ�ط�Χ�Ŀ�ʼ��
//�֡�������erase�������Ų������������ָ���Ԫ������
//��ĩβԪ��֮�������Ԫ��
//------------------------------------------------*/
//
////-------------------------------------------------------------------------
//
////��ײ���
////GLboolean CheckCollision(GameObject &one, GameObject &two);//AABB-AABB
////Collision CheckCollision(BallObject &one, GameObject &two);//AABB-Circle
//////ײ�������ж�
////Direction VectorDirection(glm::vec2 closest);
////
////void Game::DoCollisions()
////{
////	for (GameObject &box : this->Levels[this->Level].Planes)
////	{
////		if (!box.Destroyed)
////		{
////			Collision collision = CheckCollision(*Ball, box);
////			if (std::get<0>(collision))//��get<����>����Ԫ��tuple,0�����ǵ�һ��GLboolean
////			{
////				//���ש�鲻��ʵ��
////				if (!box.IsSolid)
////				{
////					box.Destroyed = GL_TRUE;
////					this->SpawnPowerUps(box);
////					SoundEngine->play2D("./resources/music/bleep.mp3", GL_FALSE);
////				}
////				else
////				{
////					//���ײ��ʵ��ש�飬ʵ�ֻζ�Ч��
////					ShakeTime = 0.05f;
////					Effects->Shake = GL_TRUE;
////					SoundEngine->play2D("./resources/music/solid.wav", GL_FALSE);
////				}
////				//��ײ����
////				Direction dir = std::get<1>(collision);
////				glm::vec2 diff_vector = std::get<2>(collision);
////				//��PassThrough���������ʵ��ש�鲻������ײ����
////				if (!(Ball->PassThrough && !box.IsSolid))
////				{
////					if (dir == LEFT || dir == RIGHT)//ˮƽ������ײ
////					{
////						Ball->Velocity.x = -Ball->Velocity.x;//��תˮƽ�ٶ�
////						//�ض�λ
////						GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);//�����뷽��ĳ���
////						if (dir == LEFT)
////							Ball->Position.x += penetration;//��������
////						else
////							Ball->Position.x -= penetration;//��������
////					}
////					else //��ֱ������ײ
////					{
////						Ball->Velocity.y = -Ball->Velocity.y;//��ת��ֱ�ٶ�
////						//�ض�λ
////						GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
////						if (dir == UP)
////							Ball->Position.y -= penetration;//��������
////						else
////							Ball->Position.y += penetration;//��������
////					}
////				}
////			}
////		}
////	}
////	//���ש����ײ��ͬʱ���͵��ߵĽӴ�
////	for (PowerUp &powerUp : this->PowerUps)
////	{
////		if (!powerUp.Destroyed)
////		{
////			if (powerUp.Position.y >= this->Height)//���ߵ�����Ļ������
////				powerUp.Destroyed = GL_TRUE;
////			if (CheckCollision(*Player, powerUp))
////			{
////				//�����뵲��Ӵ�������
////				ActivatePowerUp(powerUp);
////				powerUp.Destroyed = GL_TRUE;
////				powerUp.Activated = GL_TRUE;
////				SoundEngine->play2D("./resources/music/powerup.wav");
////			}
////		}
////	}
////	//ͬʱ�����ӵ���ײ
////	Collision result = CheckCollision(*Ball, *Player);
////	if (!Ball->Stuck && std::get<0>(result))
////	{
////		//�������ײ�ڰ��ӵ���໹���Ҳ�,���ݴ˸ı�����ٶȷ���
////		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
////		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
////		GLfloat percentage = distance / (Player->Size.x / 2);
////		//���ݽ���ƶ�
////		GLfloat strength = 2.0f;
////		glm::vec2 oldVelocity = Ball->Velocity;
////		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;//�����������ԽԶ���ٶȷ���Խ��
////		//��ת���y�����ٶ�
////		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
////		Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
////
////		//�����stuck��������Ϊ�Լ���sticky���ԣ�sticky��������stuckΪ�棬��ճ��
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
////	//x�᷽����ײ��
////	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
////		two.Position.x + two.Size.x >= one.Position.x;
////	// y�᷽����ײ��
////	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
////		two.Position.y + two.Size.y >= one.Position.y;
////	// ֻ��������������ײʱ����ײ
////	return collisionX && collisionY;
////}
////
//////��ײ��⣨AABB-Circle collision��
////Collision CheckCollision(BallObject &one, GameObject &two)
////{
////	// ��ȡԲ������ 
////	glm::vec2 center(one.Position + one.Radius);
////	// ����AABB����Ϣ�����ġ���߳���
////	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
////	glm::vec2 aabb_center(
////		two.Position.x + aabb_half_extents.x,
////		two.Position.y + aabb_half_extents.y
////	);
////	// ��ȡ�������ĵĲ�ʸ��
////	glm::vec2 difference = center - aabb_center;
////	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
////	// AABB_center����clamped�����͵õ�����ײ���Ͼ���Բ����ĵ�closest
////	glm::vec2 closest = aabb_center + clamped;
////	// ���Բ��center�������closest��ʸ�����ж��Ƿ� length <= radius
////	difference = closest - center;
////	if (glm::length(difference) <= one.Radius)
////		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
////	else
////		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));//make_tuple���ڳ�ʼ��tuple����
////}
////
//////ײ�������ж�
////Direction VectorDirection(glm::vec2 target)
////{
////	glm::vec2 compass[] = {
////		glm::vec2(0.0f,1.0f),	//��
////		glm::vec2(1.0f,0.0f),	//��
////		glm::vec2(0.0f,-1.0f),	//��
////		glm::vec2(-1.0f,0.0f)	//��
////	};
////	GLfloat max = 0.0f;
////	GLuint best_match = -1;
////	for (GLuint i = 0; i < 4; i++)
////	{
////		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);//���
////		if (dot_product > max)		//ȡ������ģ���Ϊ��ײ�����ڵ�һ��
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
////			if (std::get<0>(collision))//��get<����>����Ԫ��tuple,0�����ǵ�һ��GLboolean
////			{
////				//���ש�鲻��ʵ��
////				if (!box.IsSolid)
////				{
////					box.Destroyed = GL_TRUE;
////					this->SpawnPowerUps(box);
////					SoundEngine->play2D("./resources/music/bleep.mp3", GL_FALSE);
////				}
////				else
////				{
////					//���ײ��ʵ��ש�飬ʵ�ֻζ�Ч��
////					ShakeTime = 0.05f;
////					Effects->Shake = GL_TRUE;
////					SoundEngine->play2D("./resources/music/solid.wav", GL_FALSE);
////				}
////				//��ײ����
////				Direction dir = std::get<1>(collision);
////				glm::vec2 diff_vector = std::get<2>(collision);
////				//��PassThrough���������ʵ��ש�鲻������ײ����
////				if (!(Ball->PassThrough && !box.IsSolid))
////				{
////					if (dir == LEFT || dir == RIGHT)//ˮƽ������ײ
////					{
////						Ball->Velocity.x = -Ball->Velocity.x;//��תˮƽ�ٶ�
////						//�ض�λ
////						GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);//�����뷽��ĳ���
////						if (dir == LEFT)
////							Ball->Position.x += penetration;//��������
////						else
////							Ball->Position.x -= penetration;//��������
////					}
////					else //��ֱ������ײ
////					{
////						Ball->Velocity.y = -Ball->Velocity.y;//��ת��ֱ�ٶ�
////						//�ض�λ
////						GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
////						if (dir == UP)
////							Ball->Position.y -= penetration;//��������
////						else
////							Ball->Position.y += penetration;//��������
////					}
////				}
////			}
////		}
////	}
////	//���ש����ײ��ͬʱ���͵��ߵĽӴ�
////	for (PowerUp &powerUp : this->PowerUps)
////	{
////		if (!powerUp.Destroyed)
////		{
////			if (powerUp.Position.y >= this->Height)//���ߵ�����Ļ������
////				powerUp.Destroyed = GL_TRUE;
////			if (CheckCollision(*Player, powerUp))
////			{
////				//�����뵲��Ӵ�������
////				ActivatePowerUp(powerUp);
////				powerUp.Destroyed = GL_TRUE;
////				powerUp.Activated = GL_TRUE;
////				SoundEngine->play2D("./resources/music/powerup.wav");
////			}
////		}
////	}
////	//ͬʱ�����ӵ���ײ
////	Collision result = CheckCollision(*Ball, *Player);
////	if (!Ball->Stuck && std::get<0>(result))
////	{
////		//�������ײ�ڰ��ӵ���໹���Ҳ�,���ݴ˸ı�����ٶȷ���
////		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
////		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
////		GLfloat percentage = distance / (Player->Size.x / 2);
////		//���ݽ���ƶ�
////		GLfloat strength = 2.0f;
////		glm::vec2 oldVelocity = Ball->Velocity;
////		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;//�����������ԽԶ���ٶȷ���Խ��
////		//��ת���y�����ٶ�
////		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
////		Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
////
////		//�����stuck��������Ϊ�Լ���sticky���ԣ�sticky��������stuckΪ�棬��ճ��
////		Ball->Stuck = Ball->Sticky;
////
////		SoundEngine->play2D("./resources/music/bleep.wav");
////	}
////}

//��ײ���
//�ɻ����ӵ�
GLboolean CheckCollision(PlaneObject &one, BulletObject &two)
{
		//x�᷽����ײ
		bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
			two.Position.x + two.Size.x >= one.Position.x;
		// y�᷽����ײ��
		bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
			two.Position.y + two.Size.y >= one.Position.y;
		// ֻ��������������ײʱ����ײ
		return collisionX && collisionY;
}
//�ɻ��ͷɻ�
GLboolean CheckCollision(PlaneObject &one, PlaneObject &two)
{
	//x�᷽����ײ
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// y�᷽����ײ��
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// ֻ��������������ײʱ����ײ
	return collisionX && collisionY;
}

void Game::DoCollisions()
{
	for (PlaneObject &plane : this->Levels[this->Level].Planes)
	{
		if (!plane.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, plane);	//����Ի��͵л���ײ
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
					GLboolean collision = CheckCollision(*Player, bullet);	//����Ի����ӵ���ײ
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
					GLboolean collision = CheckCollision(plane, bullet);	//����Ի����ӵ���ײ
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