

//shader 操作
/*
	// 编译顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	// 编译几何着色器
		...
	// 编译片段着色器
		...
	// 链接着色器程序
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	//获得 shader 编译器 link 结果
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "Program Link Error:\n" << infoLog << std::endl;
	}


	//获得 shader 里变量 lineWidth 位置 
	int lineWidthLoc = glGetUniformLocation(shaderProgram, "lineWidth");

	// 传递 Uniform . 传不同的数据,对应不同的传递方法: glUniform2f glUniform2fv glUniform2i  glUniform2iv .....
	glUniform1f(lineWidthLoc, 10.0f); // 一个 float 
	glUniform2f(viewportSizeLoc, (float)viewportWidth, (float)viewportHeight); // 两个 float 

	对应 shader 声明
	----------------------
        uniform float lineWidth; 
        uniform vec2 viewportSize;
	----------------------






*/


