//todo 
/*


*/


//assimp 库
/*
Assimp（Open Asset Import Library）是一个轻量级、跨平台的 3D 模型导入库，
它的核心价值是屏蔽不同 3D 模型格式的差异。比如你拿到一个.obj、.fbx、.glb、.3ds甚至.blend格式的 3D 模型
，不用分别写解析代码，只用调用 Assimp 的接口，就能得到统一格式的模型数据（顶点、纹理、材质、骨骼动画等）
*/

//GLAD 库
/*
GLAD（Loading All OpenGL Definitions）是一个开源的C/C++库(替代之前的 glew )，专门用于管理OpenGL函数指针。其主要作用包括：
自动加载OpenGL函数指针‌
	OpenGL规范仅提供函数声明，具体实现由显卡驱动提供。GLAD在运行时自动查询并加载与指定OpenGL版本相关的函数指针，简化了函数调用过程。

跨平台支持‌
	GLAD支持Windows、Linux、macOS等主流操作系统，确保代码在不同平台上的一致性。
灵活配置‌
	用户只需指定所需的OpenGL版本和扩展（如GL_VERSION_4_5），GLAD会自动生成相应的函数指针代码。
简化OpenGL初始化‌
	在使用GLFW等窗口库时，GLAD通常与GLFW一起使用，通过gladLoadGLLoader初始化OpenGL函数指针。




*/

/*
GLFW（Graphics Library Framework）是一款轻量、跨平台的 C 语言库，核心定位是为 OpenGL / OpenGL ES / Vulkan 
	等图形 API 提供「基础运行环境支撑」
简单说：OpenGL 只负责图形渲染，而 GLFW 帮你搞定「窗口创建、OpenGL 上下文管理、输入 / 事件处理」
	等所有渲染之外的基础工作。
它是现代 OpenGL 开发的「标配窗口库」，也是老旧的 GLUT/FreeGLUT 的最佳替代方案
源码  https://github.com/glfw/glfw


GLEW（OpenGL Extension Wrangler Library，OpenGL 扩展管理库）是专门解决OpenGL 高版本功能 / 扩展函数调用问题的工具库，
	核心作用是简化 OpenGL 扩展函数的加载与管理，让开发者无需手动处理底层的
	函数地址获取、跨平台 / 跨显卡兼容性等繁琐问题。
官网 https://glew.sourceforge.net/
源码 https://github.com/nigels-com/glew  
	仓库的代码是要用 makefile 脚本生成，可以直接在 readme 里找到源码的下载包链接下载，开箱即用
*/