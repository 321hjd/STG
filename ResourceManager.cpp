/*******************************************************************
** This code is part of STG
** ���ܣ��������е��ļ���Դ��������ɫ����������Ƶ���ؿ����ݡ��浵����
** ʹ�ÿ⣺ϵͳ��iostream��sstream��fstream�����߿�stb_image��ͷ�ļ�ResourceManager
** ��д�� ����������������������������
******************************************************************/

#include "ResourceManager.h"

#include<iostream>
#include<sstream>
#include<fstream>

//ͨ������STB_IMAGE_IMPLEMENTATION��Ԥ���������޸�ͷ�ļ�������ֻ������صĺ�������Դ�룬�൱�ڰ����ͷ�ļ���Ϊһ�� .cpp �ļ�
#define STB_IMAGE_IMPLEMENTATION
#include"includes/stb_image.h"


//��ʼ����̬����
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader>	 ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const GLchar * vShaderFile, const GLchar * fShaderFile, const GLchar * gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);//������ɫ����Դ����nameΪkey����mapӳ��
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar * file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	//ɾ�����е���ɫ��
	for (auto iter : Shaders)			//for range��������������auto��ʶ�����Ը��ݳ�ʼֵ�ƶ���������
		glDeleteProgram(iter.second.ID);//iter.first��key��second��value
	//ɾ�����е�����
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar * vShaderFile, const GLchar * fShaderFile, const GLchar * gShaderFile)
{
	//���ļ�·��������ɫ����Դ����
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		//���ļ�
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		//���ļ��Ļ������ݶ�ȡ������
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		//�ر��ļ�
		vertexShaderFile.close();
		fragmentShaderFile.close();
		//��stream����ת��Ϊstring
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//���geometry��ɫ���ļ�·�����ڣ�Ҳ����һ��geometry��ɫ��
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	//��string����תΪ�ַ���,�����ַ����׵�ַ
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = nullptr;
	if (gShaderFile != nullptr)
		gShaderCode = geometryCode.c_str();
	//������ɫ������
	Shader shader;
	//����ɫ��Դ���ͽ�Compile�������Ӳ�����
	shader.Compile(vShaderCode, fShaderCode, gShaderCode != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar * file, GLboolean alpha)
{
	//�����������
	Texture2D texture;
	if (alpha)					//�����Ҫ�������������ʾ�������һ����Ҫ�޸ĸ�ʽ
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	//����ͼƬ
	int width, height, nrChannels;
	/*-----------------------------
	stbi_load����
	���ܣ�����ͼƬ
	����1��ͼƬ�ļ�λ��
	����2��3��4��ͼƬ���ߡ���ɫͨ������
	------------------------------*/
	unsigned char* image = stbi_load(file, &width, &height, &nrChannels, 0);//ͼ������
	//��������
	texture.Generate(width, height, image);
	//����������ͷ�ͼ���ڴ�
	stbi_image_free(image);
	return texture;
}
