//using namespace std;

#include "test_model_loading.h"


namespace MODEL_TEST
{








	//{
	//	Model ourModel(FileSystem::getPath("resources/objects/vampire/dancing_vampire.dae"));
	//	Animation danceAnimation(FileSystem::getPath("resources/objects/vampire/dancing_vampire.dae"), &ourModel);
	//	Animator animator(&danceAnimation);
	//}


	
	glm::mat4 GetViewMatrix()
	{
		glm::vec3 Position{0.0f, 0.0f, 100.0f};
		glm::vec3 Front{ 0.0f, 0.0f, -1.0f };
		glm::vec3 Up{0.0f, 1.0f, 0.0f };

		return glm::lookAt(Position, Position + Front, Up);
	}
	
	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	void runAni()
	{
		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		int SCR_WIDTH = 1080;
		int SCR_HEIGHT = 960;
		// glfw window creation
		// --------------------
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "xxxxxx", nullptr, nullptr);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//glfwSetCursorPosCallback(window, mouse_callback);
		//glfwSetScrollCallback(window, scroll_callback);

		// tell GLFW to capture our mouse
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



		glewExperimental = GL_TRUE; //请注意，我们在初始化GLEW之前设置glewExperimental变量的值为GL_TRUE，这样做能让GLEW在管理OpenGL的函数指针时更多地使用现代化的技术，如果把它设置为GL_FALSE的话可能会在使用OpenGL的核心模式时出现一些问题
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return;
		}


		/*
图像文件（如 PNG/JPG/BMP）的 Y 轴定义绝大多数图像文件格式（以及图像处理软件，如 PS、画图工具）
	遵循「自上而下」的 Y 轴方向,图像的左上角是纹理坐标的原点 (0, 0), Y 轴正方向是「向下」延伸，右下角对应纹理坐标 (1, 1)
	简单说：图像文件存储像素数据时，是先存第一行（顶部），再存第二行，直到最后一行（底部）。
3D 图形 API（如 OpenGL/Vulkan/Direct3D）的纹理 Y 轴定义所有主流 3D 图形 
	API 在处理纹理映射（将纹理贴到 3D 模型表面）时，遵循「自下而上」的 Y 轴方向（基于纹理坐标 UV 系统）：
	纹理的左下角是 UV 坐标的原点 (0, 0),Y 轴正方向是「向上」延伸，右上角对应 UV 坐标 (1, 1)
	这是 3D 图形渲染的标准约定，与模型的顶点 UV 坐标绑定一致。	

图形 API 与图像文件的「Y 轴方向不一致」,两个体系对纹理的「Y 轴正方向」定义完全相反，不翻转的话纹理会出现上下颠倒的问题
		*/
		// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
		stbi_set_flip_vertically_on_load(true);//必须在 stbi_load() 之前调用
		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);

		// build and compile shaders
		// -------------------------
		Shader ourShader(
			FileSystem::getPath("resources/shaders/anim_model.vs").c_str(),
			FileSystem::getPath("resources/shaders/anim_model.fs").c_str());


		// load models
		// -----------
		Model ourModel(FileSystem::getPath("resources/models/obj/vampire/dancing_vampire.dae"));
		Animation danceAnimation(FileSystem::getPath("resources/models/obj/vampire/dancing_vampire.dae"), &ourModel);
		Animator animator(&danceAnimation);


		// draw in wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// render loop
		// -----------
		while (!glfwWindowShouldClose(window))
		{
			// per-frame time logic
			// --------------------
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// input
			// -----
			//processInput(window);

			//run ani
			animator.UpdateAnimation(deltaTime);
			//animator.UpdateAnimation(0);

			// render
			// ------
			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// don't forget to enable shader before setting uniforms
			ourShader.use();

			float camera_ZOOM = 45.0f;
			//float camera_ZOOM = 60.0f;
			
			// 投影矩阵一个重要的目地是把 顶点 裁减并映射到 [-1,1] 的区间里
			// view/projection transformations 创建透视投影矩阵
			glm::mat4 projection = glm::perspective(
				// 参数1：垂直视场角（Vertical Field of View, FOV）,控制相机 “视野的开阔程度”，
				// 是垂直方向上（屏幕上下方向）的视野角度，角度越大，视野越开阔（能看到更多场景），画面中的物体看起来越小；
				// 角度越小，视野越狭窄，类似望远镜效果，物体看起来越大。
				// 水平视场角可由计算得出. 水平视场半角 和 垂直视场半角的比值  与 传感器的半宽和半高比值相等
				glm::radians(camera_ZOOM),         

				//参数 2：屏幕宽高比（Aspect Ratio）
				// 当前渲染窗口 / 屏幕的 “宽度 ÷ 高度” 的比值，
				// 用于保证 3D 场景在屏幕上显示时不会出现拉伸变形（比如避免圆形变成椭圆、方形变成矩形）。
				(float)SCR_WIDTH / (float)SCR_HEIGHT,
				
				// 参数3：近裁剪面（Near Clipping Plane）
				// 相机视锥体内（可见区域）的最近距离阈值，
				// 距离相机比该值更近的物体 / 场景会被裁剪掉（不渲染、不可见）
				0.1f,

				// 参数4：远裁剪面（Far Clipping Plane）
				//相机视锥体内（可见区域）的最远距离阈值，
				// 距离相机比该值更远的物体 / 场景会被裁剪掉（不渲染、不可见）	
				500.0f                              
			);
		
			// 创建一个平行矩阵,这个是不行的，需要 正交投影
			//glm::mat4 parallelMatrix = glm::mat4(1.0f);
			//parallelMatrix[0][0] = 1.0f; // X轴缩放
			//parallelMatrix[1][1] = 1.0f; // Y轴缩放
			//parallelMatrix[2][2] = 1.0f; // Z轴缩放
			//parallelMatrix[3][3] = 1.0f;

			// 投影矩阵一个重要的目地是把 顶点 裁减并映射到 [-1,1] 的区间里
			// 创建正交投影矩阵（正交投影无"近大远小"效果，物体大小不随距离变化，适用于2D渲染/2.5D游戏/UI绘制等场景）
			glm::mat4 orthoMatrix = glm::ortho(
				-2/2.0f,  // 参数1：左裁剪平面（Left Clipping Plane）：视口左侧的x坐标边界，小于该值的内容会被裁剪（不渲染）
				2/2.0f,   // 参数2：右裁剪平面（Right Clipping Plane）：视口右侧的x坐标边界，大于该值的内容会被裁剪（不渲染）
				-2/2.0f,  // 参数3：下裁剪平面（Bottom Clipping Plane）：视口底部的y坐标边界，小于该值的内容会被裁剪（不渲染）
				2/2.0f,   // 参数4：上裁剪平面（Top Clipping Plane）：视口顶部的y坐标边界，大于该值的内容会被裁剪（不渲染）
				0.1f,    // 参数5：近裁剪平面（Near Clipping Plane）：视口前方的z坐标边界（距离相机最近，必须大于0），小于该值的内容会被裁剪（不渲染）
				100.0f   // 参数6：远裁剪平面（Far Clipping Plane）：视口后方的z坐标边界（距离相机最远），大于该值的内容会被裁剪（不渲染）
			);

			glm::mat4 view = GetViewMatrix();// camera.GetViewMatrix;
			//ourShader.setMat4("projection", projection); //透视
			ourShader.setMat4("projection", orthoMatrix ); //正交
			ourShader.setMat4("view", view);

			auto transforms = animator.GetFinalBoneMatrices();
			for (int i = 0; i < transforms.size(); ++i)
				ourShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


			// render the loaded model, 当前模型顶点取值范围应该是 [-1,1]
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));  //模型 z 轴有小于0的,前移
			model = glm::scale(model, glm::vec3(.3f, .3f, .3f));	// it's a bit too big for our scene, so scale it down
			ourShader.setMat4("model", model);

			/*
	设置顶点裁剪空间坐标（gl_Position是顶点着色器的内置输出变量，必须赋值）
	变换流程：局部空间（蒙皮后）→ 世界空间 → 观察空间 → 裁剪空间
	mat4 viewModel = view * model; // 视图矩阵(将世界空间顶点转换为观察空间) * 模型矩阵(局部空间顶点转换为世界空间坐标)
	gl_Position =  projection *  viewModel * totalPosition; //投影矩阵（用于将视锥体中的顶点转换为裁剪空间坐标） * 模型顶点坐标 * 蒙皮骨格权重变换  
		注意：矩阵乘法顺序不可颠倒（GLSL中矩阵为列主序，变换顺序从右往左）
			projection * (view * (model * totalPosition))，遵循「矩阵在左，列向量在右」

			同时,在顶点片断里,totalPosition 是 列向量(vec4),所以在最右边

        // 骨骼蒙皮核心计算：
        // 1. 将原始顶点位置转换为齐次坐标，乘以当前骨骼的最终变换矩阵，得到该骨骼影响下的顶点局部位置
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(pos, 1.0f);
		
        // 2. 将该骨骼影响的顶点位置乘以对应权重，累加到总位置中（权重控制骨骼影响程度）
        totalPosition += localPosition * weights[i];


*/

			ourModel.Draw(ourShader);






			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}

	void runModel()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		int SCR_WIDTH = 1080;
		int SCR_HEIGHT = 960;
		// glfw window creation
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "xxxxxx", nullptr, nullptr);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return;
		}
		// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
		//stbi_set_flip_vertically_on_load(true);

		glEnable(GL_DEPTH_TEST);

		// -------------------------


		// build and compile shaders
		Shader ourShader(
			FileSystem::getPath("resources/shaders/1.model_loading.vs").c_str(),
			FileSystem::getPath("resources/shaders/1.model_loading.fs").c_str());


		// load models
		// -----------
		Model ourModel(FileSystem::getPath("resources/models/obj/backpack/backpack.obj"));

		// render loop
		// -----------
		while (!glfwWindowShouldClose(window))
		{
			// per-frame time logic
			// --------------------
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// input
			// -----
			//processInput(window);


			// render
			// ------
			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// don't forget to enable shader before setting uniforms
			ourShader.use();

			//float camera_ZOOM = 45.0f;
			float camera_ZOOM = 65.0f;
			// view/projection transformations
			glm::mat4 projection = glm::perspective(
				glm::radians(camera_ZOOM),
				(float)SCR_WIDTH / (float)SCR_HEIGHT,
				0.1f, 500.0f);

			glm::mat4 view = GetViewMatrix();// camera.GetViewMatrix;
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);

			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(.3f, .3f, .3f));	// it's a bit too big for our scene, so scale it down
			ourShader.setMat4("model", model);
			ourModel.Draw(ourShader);


			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}

	void test()
	{
		//runModel();	
		runAni();
	}
}


