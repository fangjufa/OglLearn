#include<iostream>
#include<assert.h>
#include"OglWindow.hpp"
#include"GLEW\glew.h"
#include"ShaderResource.hpp"
#include"GLEW\wglew.h"

using namespace std;

oglWindow oglWin;
bool      g_ContinueRendering= true;
int       nStep;
HDC       g_hDC;
HGLRC     g_hRC;
GLuint    m_VertexShader;
GLuint    m_FragmentShader;
GLuint    m_Program;
GLuint    kVertexInputPosition;
float*    vertexArray;
int*      indexArray;
GLuint    m_VertexBuffer;
GLuint    m_IndexBuffer;


static GLuint CompileShader(GLenum type, const char* sourceText)
{
	GLuint ret = glCreateShader(type);
	glShaderSource(ret, 1, &sourceText, NULL);
	glCompileShader(ret);
	if (ret != GL_ZERO)
		printf("Compile shader success.\r\n");
	else
		printf("Compile shader failed.Type:%d.\r\n", type);
	return ret;
}

///创建Shader资源
void SetupShader()
{
	m_VertexShader = CompileShader(GL_VERTEX_SHADER, VERTEX_SHADER_SRC());
	m_FragmentShader = CompileShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC());

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		printf("Create shader failed.error:%s.", glewGetErrorString(err));
	else
		printf("Create shader success.");

	// Link shaders into a program and find uniform locations
	m_Program = glCreateProgram();
	if (m_Program != 0)
		printf("Shader program not null.\r\n");

	//将变量kVertexInputPosition与shader中的pos变量绑定,pos变量在shader中的位置赋给kVertexInputPosition
	//该函数与下面的glBindFragDataLocation用处不一样，该函数是绑定Vertex shader中的变量的。
	glBindAttribLocation(m_Program, kVertexInputPosition, "pos");
	glAttachShader(m_Program, m_VertexShader);
	glAttachShader(m_Program, m_FragmentShader);

	//绑定fragment shader中的变量。
	//glBindFragDataLocation(m_Program, 0, "fragColor");
	
	//加上这句话之后，glBindAttribLocation的语句才开始生效。
	glLinkProgram(m_Program);

	GLint status = 0;
	glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
	assert(status == GL_TRUE);


	//m_UniformCoeff = glGetUniformLocation(m_Program, "coeff");
	GLenum error = glGetError();
	printf("Get shader variable location error:%d.\r\n", error);
}


///创建VBO，VAO等。
void SetupBuffers()
{
	//初始化顶点数组和索引数组。
	vertexArray = new float[12]{ -1.f,-1.f, 0,-1.f, 1.f, 0, 1.f, 1.f, 0, 1.f,-1.f, 0 };
	//逆时针
	indexArray = new int[6]{ 0,2,1,3,2,0 };
	// Create vertex buffer VBO
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertexArray, GL_STATIC_DRAW);

	//创建索引缓存
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indexArray, GL_STATIC_DRAW);

	assert(glGetError() == GL_NO_ERROR);
	printf("Create resource success.\n");
}

void ChangeSize(int w,int h) {}

void mainLoop()
{
	//这个函数是必须的，不然会没有效果。
	wglMakeCurrent(g_hDC, g_hRC);

	glClear(GL_COLOR_BUFFER_BIT);

	//glViewport(0, 0, w, h);//指明视口
	GLenum glEnum = glGetError();
	if (glEnum != GL_NO_ERROR)
		printf("First GL error:%d.\r\n", glEnum);

	//设置使用的shader。如果要渲染多个物体并且不同的shader的话，还可以继续加glUseProgram
	glUseProgram(m_Program);

	//下面的几句话，操作的数组是在glBindBuffer中设置GL_ARRAY_BUFFER设定的。
	//int positionSlot = glGetAttribLocation(m_Program, "pos");
	//开启GPU读取顶点着色器的数据，如果不写这句，GPU是无法读取数据的，因为CPU与GPU之间的通道默认是关闭的。
	glEnableVertexAttribArray(kVertexInputPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	//给shader中传递Position。
	//最后一个参数是指偏移量，因为只有这一个数组，所以偏移量为0.
	glVertexAttribPointer(kVertexInputPosition, 3/*每个顶点属性的组件数量，比如每个顶点由3个float值组成*/, GL_FLOAT, GL_FALSE, 0/*步长，连续两个顶点之间跨越的值*/, (char*)0);
	

	glEnum = glGetError();
	if (glEnum != GL_NO_ERROR)
		printf("Second GL error:%d.\r\n", glEnum);
	//glEnableVertexAttribArray(kVertexInputColor);
	//glVertexAttribPointer(kVertexInputColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, kVertexSize, (char*)NULL + 12);
	//索引数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

	// Draw
	//第一个参数：三角形的形态，第二个参数，
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indexArray);


	glDisableVertexAttribArray(kVertexInputPosition);

	glEnum = glGetError();
	if (glEnum != GL_NO_ERROR)
		printf("Last GL error:%d.\r\n", glEnum);

	SwapBuffers(g_hDC);
}

void RenderScene() {}

LRESULT CALLBACK WndProc(HWND	hWnd,		// Handle For This Window
	UINT	uMsg,		// Message For This Window
	WPARAM	wParam,		// Additional Message Information
	LPARAM	lParam)		// Additional Message Information
{
	unsigned int key = 0;

	// Handle relevant messages individually
	switch (uMsg)
	{
	case WM_ACTIVATE:
	case WM_SETFOCUS:
		//RenderScene();
		return 0;
	case WM_SIZE:
		ChangeSize(LOWORD(lParam), HIWORD(lParam));
		//RenderScene();
		break;
	case WM_CLOSE:
		g_ContinueRendering = false;
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		key = (unsigned int)wParam;
		if (key == 32)
		{
			nStep++;
			if (nStep > 5)
				nStep = 0;
		}
		return 0;
	default:
		// Nothing to do now
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int main()
{
	if (oglWin.Create(800, 600, WndProc))
	{
		PIXELFORMATDESCRIPTOR pfd;
		g_hDC = GetDC(oglWin._hWnd);

		int nPixCount = 0;

		// Specify the important attributes we care about
		int pixAttribs[] = {
			WGL_SUPPORT_OPENGL_ARB, 1, // Must support OGL rendering
			WGL_DRAW_TO_WINDOW_ARB, 1, // pf that can run a window
			//WGL_ACCELERATION_ARB,   1, // must be HW accelerated
			WGL_RED_BITS_ARB,       8, // 8 bits of red precision in window
			WGL_GREEN_BITS_ARB,     8, // 8 bits of green precision in window
			WGL_BLUE_BITS_ARB,      8, // 8 bits of blue precision in window
			WGL_DEPTH_BITS_ARB,     16, // 16 bits of depth precision for window
			WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB, // pf should be RGBA type
			0 }; // NULL termination

		int nPixelFormat = 2;

		// Ask OpenGL to find the most relevant format matching our attribs
		// Only get one format back.
		//wglChoosePixelFormatARB(g_hDC, &pixAttribs[0], NULL, 1, &nPixelFormat, (UINT*)&nPixCount);
		//if (nPixelFormat == -1)
		//	printf("couldn't find a format.");
		//printf("nPixelFormat:%d", nPixelFormat);

		SetPixelFormat(g_hDC, nPixelFormat, &pfd);
		//创建OGL上下文，并将其设置为当前的窗口。
		g_hRC = wglCreateContext(g_hDC);
		//GLint attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB,  3,
		//	WGL_CONTEXT_MINOR_VERSION_ARB,  3,
		//	0 };
		//g_hRC = wglCreateContextAttribsARB(g_hDC, 0, attribs);
		//if (g_hRC == NULL)
		//{
		//	printf("!!! Could not create an OpenGL 3.3 context.\n");
		//	attribs[3] = 2;
		//	g_hRC = wglCreateContextAttribsARB(g_hDC, 0, attribs);
		//	if (g_hRC == NULL)
		//	{
		//		printf("!!! Could not create an OpenGL 3.2 context.\n");
		//		attribs[3] = 1;
		//		g_hRC = wglCreateContextAttribsARB(g_hDC, 0, attribs);
		//		if (g_hRC == NULL)
		//		{
		//			printf("!!! Could not create an OpenGL 3.1 context.\n");
		//			attribs[3] = 0;
		//			g_hRC = wglCreateContextAttribsARB(g_hDC, 0, attribs);
		//			if (g_hRC == NULL)
		//			{
		//				printf("!!! Could not create an OpenGL 3.0 context.\n");
		//				printf("!!! OpenGL 3.0 and higher are not supported on this system.\n");
		//			}
		//		}
		//	}


		wglMakeCurrent(g_hDC, g_hRC);

		if (g_hDC == 0 || g_hRC == 0)
		{
			printf("创建DC和Context出现错误。");
		}
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			printf("glew初始化错误:%s.", glewGetErrorString(err));
		}

		const GLubyte  *oglVersion = glGetString(GL_VERSION);
		printf("This system supports OpenGL Version %s.\n", oglVersion);

		SetupShader();
		SetupBuffers();
		glClearColor(1, 1, 0, 1);

		ChangeSize(800, 600);
		while (g_ContinueRendering)
		{
			//这个while循环一定要有，不然窗口会卡主
			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			mainLoop();
			Sleep(0);
		}
	}
	return 0;
}