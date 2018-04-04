﻿#pragma once

#define VERTEX_SHADER_SRC()						                    \
	"#version 150\n"							                    \
	" in highp vec3 pos;\n"				  						    \
	" in highp vec2 texcoord;\n"				  				    \
	" out highp vec2 outTexcoord;\n"				  				\
	"void main()\n"													\
	"{\n"															\
	"	gl_Position = vec4(pos,1);//mvpMatrix * vec4(pos,1);\n"     \
	"	outTexcoord = texcoord;\n"								    \
	"}\n"															\

// Simple fragment shader source
#define FRAGMENT_SHADER_SRC()						\
	"#version 150\n"								\
	"in lowp vec2 outTexcoord;\n"					\
	"uniform sampler2D mSampler;\n"					\
	"out lowp vec4 fragColor;\n"					\
	"void main()\n"									\
	"{\n"											\
	"    fragColor = texture2D(mSampler,outTexcoord);//vec4(0,0,0.5,1);\n"		\
	"}\n"											\
//本来提示未申明的标记，加了一行空行就可以了，神奇。