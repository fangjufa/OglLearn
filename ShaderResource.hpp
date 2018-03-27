#pragma once

#define VERTEX_SHADER_SRC()						                    \
	"#version 150\n"							                    \
	"in highp vec3 pos;\n"				  						    \
	"void main()\n"													\
	"{\n"															\
	"	gl_Position = vec4(pos,1);//mvpMatrix * vec4(pos,1);\n"     \
	"}\n"															\

// Simple fragment shader source
#define FRAGMENT_SHADER_SRC()						\
	"#version 150\n"								\
	"out lowp vec4 fragColor;\n"					\
	"\n"											\
	"void main()\n"									\
	"{\n"											\
	"    fragColor = vec4(0,0,1,1);\n"		\
	"}\n"											\
//本来提示未申明的标记，加了一行空行就可以了，神奇。