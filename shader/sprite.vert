//Breakout
#version 330 core
layout (location = 0) in vec4 vertex;//用一个vec4的变量储存纹理位置和纹理坐标（各两个float）

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	TexCoords = vertex.zw;
	gl_Position = projection * model * vec4(vertex.xy,0.0f,1.0f);
}