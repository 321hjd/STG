#pragma once

/*******************************************************************
** This code is part of STG
** 功能：着色器生成和配置
** 使用库：工具库glad
** 编写于 ？？？？？？？？？？
******************************************************************/

#ifndef TEXTURE_H
#define TEXTURE_H

#include"includes/glad.h"



//纹理类：包含配置纹理各个属性的成员、从资源文件加载图片并生成纹理
class Texture2D
{
public:
	//纹理对象的ID
	GLuint ID;
	//纹理图片宽、高(像素)
	GLuint Width, Height;
	//纹理格式
	GLuint Internal_Format;	//纹理对象格式
	GLuint Image_Format;	//加载图片的格式
	//纹理配置
	GLuint Wrap_S;			//S轴上的环绕模式
	GLuint Wrap_T;			//T轴上的环绕模式
	GLuint Filter_Min;		//如果纹理像素 < 屏幕像素，则采用过滤模式
	GLuint Filter_Mag;		//如果纹理像素 > 屏幕像素，则采用过滤模式
	//构造函数，绑定纹理对象
	Texture2D();
	/*---------------------------
	功能：从图片数据生成纹理
	参数1：width（纹理宽）
	参数2：height（纹理高）
	参数3：data（用于存储图片数据）
	输出：无
	编写于？？？？？？？？？？
	----------------------------*/
	void Generate(GLuint width, GLuint height, unsigned char* data);

	//将纹理绑定到当前激活的的GL_TEXTURE_2D对象
	void Bind() const;
};

#endif
