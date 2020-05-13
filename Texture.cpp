/*******************************************************************
** This code is part of STG
** 功能：着色器生成和配置
** 使用库：系统库iostream，头文件Texture
** 编写于 ？？？？？？？？？？
******************************************************************/

#include<iostream>

#include "Texture.h"


Texture2D::Texture2D() :
	Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Mag(GL_LINEAR)
{
	glGenTextures(1, &this->ID);	//生成纹理对象个数，并存储至ID
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char * data)
{
	this->Width = width;
	this->Height = height;
	//创建纹理
	glBindTexture(GL_TEXTURE_2D, this->ID);			//绑定一个2D纹理对象

	/*-------------------------------------------
	glTexImage2D函数
	功能：生成纹理
	参数1：指定纹理目标设置为GL_TEXTURE_2D意味着会生成与当前绑定
	的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和
	GL_TEXTURE_3D的纹理不会受到影响
	参数2：指定多级渐远纹理的级别
	参数3：希望的纹理存储格式
	参数4、5：图像宽、高
	参数6：总是0
	参数7、8：定义源图格式和数据类型，这里用RGB直加载图像并储存为
			char(byte)数组
	参数9：真正的图像数据-data
	--------------------------------------------*/
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);

	/*-----------------------------------------------
	glTexParameter 函数:glTexParameteri glTexParameterfv
	功能：为当前绑定的纹理对象设置环绕、过滤方式
	参数1：指定纹理目标
	参数2：指定设置的选项和应用的纹理轴：
		环绕：参数2 指定S和T轴；参数3 指定环绕方式（四种)
		过滤：参数2 指定放大和缩小；参数3 过滤方式
	-------------------------------------------------*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);
	//解绑纹理对象
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
