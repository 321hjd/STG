#pragma once

/*******************************************************************
** This code is part of STG
** 功能：管理文本渲染
** 使用库：系统库map，头文件Texture、Shader，工具库glad、glm
** 编写于 ？？？？？？？？？？？？？？
******************************************************************/

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include "includes/glad.h"
#include "includes/glm/glm.hpp"

#include "Texture.h"
#include "Shader.h"


//保存与使用FreeType加载的字符相关的所有状态信息
struct Character {
	GLuint TextureID;	//标识字形纹理的ID句柄
	glm::ivec2 Size;	//字体大小
	glm::ivec2 Bearing;	//从基线到字形左侧/顶部的偏移量
	GLuint Advance;		//前进到下一个字形的水平偏移量
};

//加载的字体所显示的文本的呈现程序类
//加载单一字体，将其处理为字符列表
class TextRenderer
{
public:
	std::map<GLchar, Character> Characters;	//将生成的字符存放进一个映射
	Shader TextShader;						//用于渲染字形的着色器
	//构造函数
	TextRenderer(GLuint width, GLuint height);
	
	/*-----------------------------------------
	功能：从给定字体预编译字符列表
	参数1：字体文件路径
	参数2：字体大小
	输出：无
	编写于 ？？？？？？？？？？？？？？
	-----------------------------------------*/
	void Load(std::string font, GLuint fontSize);
	
	/*-----------------------------------------
	功能：用编译好的字符列表渲染文本
	参数1：需要渲染的文本
	参数2：文本x坐标
	参数3：文本y坐标
	参数4：文本缩放大小
	参数5：文本颜色
	输出：无
	编写于 ？？？？？？？？？？？？？？
	------------------------------------------*/
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	GLuint VAO, VBO;	//渲染所需顶点对象
};


#endif