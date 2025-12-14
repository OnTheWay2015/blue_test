#include "windows.h"
#include <iostream>



//GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

#pragma comment(lib,"glew32.lib")

//using namespace std;

// Window dimensions
 
// Other Libs
#include "..\SimpleOpenGLImageLibrary\src\SOIL.h"


#include "glm/glm.hpp"
 
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
 
const char *vertexShaderSource1 = 0;
const char *fragmentShaderSource1 = 0;
GLuint VAO[2];
GLuint VBO[2];
GLuint EBO[2];
GLuint texture1;
GLuint texture;

GLuint shaderProgram;
int readfile1(const char *file, const char **buff)
{
	FILE *stream;

	if ((stream = fopen(file, "r")) == NULL)
	{
		fprintf(stderr, "cannot open output file.\n");
		return 1;
	}

	fseek(stream, 0, SEEK_END);
	int lSize = ftell(stream) + 1;
	*buff = (char*)malloc(lSize);
	memset((void*)*buff, 0, lSize);
	/*seek to the beginning of the file*/
	fseek(stream, 0, SEEK_SET);
	/*read the data and display it*/
	fread((void*)*buff, 1, lSize, stream);
	printf("%s\n\n", *buff);
	fclose(stream);
	return 0;
}
void key_callback1(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

}

/*
设置数据以后，我们需要用 glGenVertexArrays 创建一个Vertex Array Object, 然后使用glBindVertexArray绑定VAO，一旦VAO绑定后，
使glGenBuffers 创建一个Vertex Buffer Object, 当然仍然需要使用glBindBuffer绑定VBO；
顺序如下：
1. Generate Vertex Array Object
2. Bind Vertex Array Object
3. Generate Vertex Buffer Object
4. Bind Vertex Buffer Object


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


bool initflag = false;
GLuint vertexShader;
GLuint fragmentShader;


//char *_test_img = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\test.jpg";
char *_test_img1 = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect.png";
char *_test_img = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\test.png";
//char *_test_img = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect.png";
//char *_test_img = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_img_test.png";

int rot = 0;
void drawwxx();
void draww1();
void drawact()
{
	if (!initflag)
	{
		draww1();
	}
	
	drawwxx();
}


void draww1()
{
 
	//vertexShader
	

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource1, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//fragmentShader
	 

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	memset(infoLog, 0, 512);
	success = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::fragmentShader::COMPILATION_FAILED\n" << infoLog << std::endl;
	}



	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::shaderProgram::Link_FAILED\n" << infoLog << std::endl;
	} 

	
	GLfloat vertices1[] = {//tex, rgb, uv
		-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		
		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		
		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,		
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
	};


	GLfloat vertices2[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	};


	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 2 // First Triangle		 
		, 2, 3, 0  // Second Triangle
		
		, 4, 5, 6
		, 6, 7, 4
		
		,8, 9,10
		, 10,11,8

		,12, 13,14
		, 14, 15, 12

		,16, 17,18
		,18, 19, 16
		,20, 21, 22
		,22, 23, 20
	};






	glGenVertexArrays(1, &VAO[0]);
	// 绑定VAO, VBO
	glBindVertexArray(VAO[0]);

	glGenBuffers(1, &EBO[0]);


	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 设置顶点属性指针 第一个参数和顶点shader的 layout location 对应, 第二个是当前属性元素个数
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	/* Specify that our color data is going into attribute index 1, and contains three floats per vertex */  
	//6 * sizeof(GLfloat) 步长,每个顶点数据长度, 注意最后一个参数,偏移？？
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); 
	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);



	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(2);


	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(_test_img, &width, &height, 0, SOIL_LOAD_RGB);


	int width1, height1;
	unsigned char* image1 = SOIL_load_image(_test_img1, &width1, &height1, 0, SOIL_LOAD_RGB);


	// Load and create a texture 

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	 
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D); //直接在生成纹理之后调用 glGenerateMipmap。这会为当前绑定的纹理自动生成所有需要的多级渐远纹理
	
	 
	SOIL_free_image_data(image); 
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.




//------------------------ texture 2 ---------------	

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D); 


	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0); 

	//------------------------ texture 2 end ---------------	
	 
}

void drawwxx()
{ 


	glUseProgram(shaderProgram);//在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了。

	rot++;
	glm::mat4 trans; //modle -> world

	trans = glm::rotate(trans, (GLfloat)rot, glm::vec3(1.0, 0.0, 0.0));

	trans = glm::scale(trans, glm::vec3(0.2, 0.2, 0.2));
	trans = glm::translate(trans, glm::vec3(0.0f, 0.5f, 0.0f));

	GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


	glm::mat4 view;// world  -> view 
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection; // view   -> clip
	projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

	GLuint viewLoc = glGetUniformLocation(shaderProgram, "transview");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "transprojection");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); 


	// Bind Texture
	glActiveTexture(GL_TEXTURE0);  //OpenGL至少保证有16个纹理单元供你使用，也就是说你可以激活从GL_TEXTURE0到GL_TEXTRUE15。它们都是按顺序定义的，所以我们也可以通过GL_TEXTURE0 + 8的方式获得GL_TEXTURE8，这在当我们需要循环一些纹理单元的时候会很有用。
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);//对应到shader

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 1);


	glEnable(GL_DEPTH_TEST);



	initflag = true;

	glBindVertexArray(VAO[0]);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);


	glDeleteShader(vertexShader);   //对了，在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
	glDeleteShader(fragmentShader);


	//printf(" err[%d] ?\n", glGetError());  //显示当前错误重置
}




int glfwtest_more()
{

	printf("load vertexShaderSource:\n");
	readfile1("G:/source_accumulation_vs/ogltest/test/_vertexShaderSource.txt", &vertexShaderSource1);

	printf("load fragmentShaderSource:\n");
	readfile1("G:/source_accumulation_vs/ogltest/test/_fragmentShaderSource.txt", &fragmentShaderSource1);


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	//GLFWwindow * window = glfwCreateWindow(256, 256, "LearnOpenGL", nullptr, nullptr);
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


	glfwSetKeyCallback(window, key_callback1);

	while (!glfwWindowShouldClose(window))
	{
		////glfwWindowShouldClose函数从开始便检验每一次循环迭代中gLFW是否已经得到关闭指示，如果得到这样的指示，函数就会返回true,并且游戏循环停止运行，之后我们就可以关闭应用了。  
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f); //调用了glClearColor来设置清空屏幕所用的颜色。当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色。
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawact();

		glfwPollEvents();//检验是否有任务被触发了，（比如鼠标移动和键盘的输入事件），接着调用相应的函数（我们可以通过回调方法设置他们）。我们经常在循环迭代事件中处理函数  
		glfwSwapBuffers(window);//函数会交换颜色缓冲（颜色缓冲是一个GLFW窗口为每一个像素存储的颜色数值的最大缓冲） 它是这迭代中绘制的，也作为输出到显示在屏幕上   

	}
	glfwTerminate();//释放所有的资源  
	return 0;
} 


/*
· 局部空间(Local Space，或者称为物体空间(Object Space))
· 世界空间(World Space)
· 观察空间(View Space，或者称为视觉空间(Eye Space))
· 裁剪空间(Clip Space)
· 屏幕空间(Screen Space)
*/



int main(void)
{	 
	glfwtest_more();
	system("pause");
	return 0;
}
