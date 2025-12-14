#include "windows.h"
#include <iostream>
//#include <stdio.h>

 
//GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

#pragma comment(lib,"glew32.lib")

//using namespace std;
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


int readfile(const char *file, const char **buff )
{
	FILE *stream;
	 
	if ((stream = fopen(file, "r")) == NULL)
	{
		fprintf(stderr, "cannot open output file.\n");
		return 1;
	}

	fseek(stream, 0, SEEK_END);
	int lSize = ftell(stream) + 1;
	*buff = (char*)malloc( lSize);
	memset((void*)*buff, 0, lSize);
	/*seek to the beginning of the file*/
	fseek(stream, 0, SEEK_SET);
	/*read the data and display it*/
	fread((void*)*buff, 1, lSize, stream);
	printf("%s\n\n", *buff);
	fclose(stream);	
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

}

/*
3.8开始渲染
3.8.1填充（清理）屏幕
glClearColor(0.1f, 0.9f, 0.5f, 1);//指定填充屏幕的RGBA值
glClear(GL_COLOR_BUFFER_BIT);//指定要清除哪些缓冲区，GL_COLOR_BUFFER_BIT表示颜色缓冲区，GL_DEPTH_BUFFER_BIT表示深度缓冲区，GL_STENCIL_BUFFER_BIT表示模板缓冲区
3.8.2从着色器代码中获取属性信息
GLuint m_simpleProgram = programHandle;
GLuint positionSlot = glGetAttribLocation(m_simpleProgram, "Position");//从着色器源程序中的顶点着色器中获取Position属性
GLuint colorSlot = glGetAttribLocation(m_simpleProgram, "SourceColor");//从着色器源程序中的顶点着色器中获取SourceColor属性
3.8.3开启顶点属性数组
glEnableVertexAttribArray(positionSlot);
glEnableVertexAttribArray(colorSlot);
3.8.4为着色器属性赋值
GLsizei stride = sizeof(Vertex);//单个顶点的数据长度
const GLvoid *pCoords = &vertices[0].Position[0];//顶点数组中的位置数组首地址
const GLvoid *pColors = &vertices[0].Color[0];/顶点数组中的颜色数组首地址
glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, stride, pCoords);//为顶点着色器位置信息赋值，positionSlot表示顶点着色器位置属性（即，Position）；2表示每一个顶点信息由几个值组成，这个值必须位1，2，3或4；GL_FLOAT表示顶点信息的数据类型；GL_FALSE表示不要将数据类型标准化（即fixed-point）；stride表示数组中每个元素的长度；pCoords表示数组的首地址
glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColors);//同上
3.8.5渲染顶点
GLsizei vertexCount = sizeof(vertices)/sizeof(Vertex);//顶点个数
glDrawArrays(GL_TRIANGLES, 0, vertexCount);//将顶点数组使用三角形渲染，GL_TRIANGLES表示三角形， 0表示数组第一个值的位置，vertexCount表示数组长度
3.8.6渲染完毕，关闭顶点属性数组
glDisableVertexAttribArray(positionSlot);
glDisableVertexAttribArray(colorSlot);

*/

const char *vertexShaderSource = 0;
const char *fragmentShaderSource = 0;

void draww()
{
	printf("reset err[%d] ?\n", glGetError()); //第一次调用时重置

	//vertexShader
	GLuint vertexShader;

	//const char *vertexShaderSource = "layout(location = 0) in vec3 position;void main() { gl_Position = vec4(position.x, position.y, position.z, 1.0); }";

	vertexShader = glCreateShader(GL_VERTEX_SHADER);	
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[1024];
	GLchar infoLog1[1024];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

	}


	//fragmentShader
	GLuint fragmentShader;
	//const char *fragmentShaderSource = "out vec4 color; void main() { color = vec4(1.0f, 0.5f, 0.2f, 1.0f);  }";

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	memset(infoLog, 0, 1024);
	success = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
 
		std::cout << "ERROR::SHADER::fragmentShader::COMPILATION_FAILED\n" << infoLog << std::endl;
		
	}



	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); 

	/* 查看加载的shader，看看是不是和写的一样
	GLchar infoLog1[1024];
	memset(infoLog1, 0, 1024);  
	glGetShaderSource(fragmentShader, 1024, NULL, infoLog1);

	std::cout << "ERROR::SHADER::sss::COMPILATION_FAILED\n" << infoLog1 << std::endl;*/

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};



	GLuint VBO;
	glGenBuffers(1, &VBO); //缓冲有一个独一无二的ID，所以我们可以使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	
	//把之前定义的顶点数据复制到缓冲的内存中
	/*
	它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。第二个参数指定传输数据的大小(以字节为单位)；
	用一个简单的sizeof计算出顶点数据大小就行。第三个参数是我们希望发送的实际数据。第四个参数指定了我们希望显卡如何管理给定的数据
	
	VBO允许usage标示符取以下9种值：

		GL_STATIC_DRAW_ARB

		GL_STATIC_READ_ARB

		GL_STATIC_COPY_ARB

		GL_DYNAMIC_DRAW_ARB

		GL_DYNAMIC_READ_ARB

		GL_DYNAMIC_COPY_ARB

		GL_STREAM_DRAW_ARB

		GL_STREAM_READ_ARB

		GL_STREAM_COPY_ARB

	"Static”意味着VBO中的数据不会被改变（一次修改，多次使用），
	"dynamic”意味着数据可以被频繁修改（多次修改，多次使用），
	"stream”意味着数据每帧都不同（一次修改，一次使用）。
	"Draw”意味着数据将会被送往GPU进行绘制，
	"read”意味着数据会被用户的应用读取，
	"copy”意味着数据会被用于绘制和读取。
	注意在使用VBO时，只有draw是有效的，而copy和read主要将会在像素缓冲区（PBO）和帧缓冲区（FBO）中发挥作用。
	系统会根据usage标示符为缓冲区对象分配最佳的存储位置，比如系统会为GL_STATIC_DRAW_ARB和GL_STREAM_DRAW_ARB
	分配显存，GL_DYNAMIC_DRAW_ARB分配AGP，以及任何_READ_相关的缓冲区对象都会被存储到系统或者AGP中因为这样数据
	更容易读写。
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
	


	GLuint VAO;

	//generate vertex array object names
	/*
	void glGenVertexArrays(	GLsizei n, 	GLuint *arrays); 
		Parameters
		n
		Specifies the number of vertex array object names to generate.
		arrays
		Specifies an array in which the generated vertex array object names are stored.
	*/
	glGenVertexArrays(1, &VAO); 

	// 1. 绑定VAO
	glBindVertexArray(VAO);

	//// 2. 把顶点数组复制到缓冲中供OpenGL使用
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3. 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 4. 解绑VAO


	

	//glBindVertexArray(0);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::shaderProgram::Link_FAILED\n" << infoLog << std::endl;

	}

	glUseProgram(shaderProgram);//在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了。
	
	// 3. 绘制物体
	//glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glDeleteShader(vertexShader);   //对了，在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
	glDeleteShader(fragmentShader);

	printf(" err[%d]\n", glGetError());  //显示当前错误重置
}

/*
unsigned int  createList(string& str, int fontIndex, unsigned int & list1) //并不显示，函数只是创建一个显示列表，并返回
{
	HDC dc = wglGetCurrentDC();
	::SelectObject(dc, GLFont::fonts[fontIndex]);
	unsigned short length = str.size();


	glPushMatrix();
	unsigned long* otherEncode = new  unsigned long[length];
	unsigned int exIndex = 0;
	for (unsigned short i = 0; i
	{
		if (IsDBCSLeadByte((BYTE)str[i]))
		{
			otherEncode[exIndex++] = (unsigned long)((str[i] << 8) + 256 + str[i + 1]);
			++i;
		}
		else
		{
			otherEncode[exIndex++] = str[i];
		}

	}
		// MultiByteToWideChar(U)
	GLYPHMETRICSFLOAT* pgmf = new GLYPHMETRICSFLOAT[exIndex];   //轮廓字体字符集的信息

	list1 = glGenLists(exIndex);
	if (!list1)
	{//显示类表申请失败
		return 0;
	}

	for (unsigned int clIndex = 0; clIndex
	{
		wglUseFontOutlines(dc, otherEncode[clIndex], 1, list1 + clIndex, 0.0f, GLFont::fontThick[fontIndex], WGL_FONT_POLYGONS, pgmf);
	}

	glPopMatrix();
	delete[] pgmf;
	return exIndex;
}
*/
// ASCII字符总共只有0到127，一共128种字符
#define MAX_CHAR       128
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;
	if (isFirstCall) { // 如果是第一次调用，执行初始化
		// 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;
		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);
		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

int drawtext()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos2f(0.0f, 0.0f);
	drawString( "aaaa" );
	//glutSwapBuffers();
	return 0;
}


int glfwtest()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	 

	glewExperimental = GL_TRUE; //请注意，我们在初始化GLEW之前设置glewExperimental变量的值为GL_TRUE，这样做能让GLEW在管理OpenGL的函数指针时更多地使用现代化的技术，如果把它设置为GL_FALSE的话可能会在使用OpenGL的核心模式时出现一些问题
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}


	int width, height;
	glfwGetFramebufferSize(window, &width, &height); 

	/*
	glViewport数前两个参函数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素），这里我们是直接从GLFW中获取的。我们从GLFW中获取视口的维度而不设置为800*600是为了让它在高DPI的屏幕上（比如说Apple的视网膜显示屏）也能正常工作。
	我们实际上也可以将视口的维度设置为比GLFW的维度小，这样子之后所有的OpenGL渲染将会在一个更小的窗口中显示，这样子的话我们也可以将一些其它元素显示在OpenGL视口之外。
	OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，将OpenGL中的位置坐标转换为你的屏幕坐标。例如，OpenGL中的坐标(-0.5, 0.5)有可能（最终）被映射为屏幕中的坐标(200,450)。注意，处理过的OpenGL坐标范围只为-1到1，因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)。

	*/
	glViewport(0, 0, width, height); //glViewport数前两个参函数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）


	glfwSetKeyCallback(window, key_callback);

	printf( "load vertexShaderSource:\n" );
	readfile("G:/source_accumulation_vs/ogltest/test/_vertexShaderSource_single.txt", &vertexShaderSource);

	printf("load fragmentShaderSource:\n");
	readfile("G:/source_accumulation_vs/ogltest/test/_fragmentShaderSource_single.txt", &fragmentShaderSource);
	

	while (!glfwWindowShouldClose(window))
	{
		////glfwWindowShouldClose函数从开始便检验每一次循环迭代中gLFW是否已经得到关闭指示，如果得到这样的指示，函数就会返回true,并且游戏循环停止运行，之后我们就可以关闭应用了。  
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f); //调用了glClearColor来设置清空屏幕所用的颜色。当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色。
		glClear(GL_COLOR_BUFFER_BIT);

		draww();
		//drawtext();
		glfwPollEvents();//检验是否有任务被触发了，（比如鼠标移动和键盘的输入事件），接着调用相应的函数（我们可以通过回调方法设置他们）。我们经常在循环迭代事件中处理函数  
		glfwSwapBuffers(window);//函数会交换颜色缓冲（颜色缓冲是一个GLFW窗口为每一个像素存储的颜色数值的最大缓冲） 它是这迭代中绘制的，也作为输出到显示在屏幕上   

	}
	glfwTerminate();//释放所有的资源  
	return 0;
} 
 





int main1(void)
{
	glfwtest();
	system( "pause" );
	return 0;
}
