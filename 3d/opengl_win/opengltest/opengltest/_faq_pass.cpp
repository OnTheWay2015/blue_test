{
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);

glFrontFace(GL_CW); 

glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LESS);


glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
glDepthMask(GL_TRUE)		


glEnable(GL_STENCIL_TEST);
glStencilFunc(GL_ALWAYS, 1, 0xFF);    
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);   
glStencilMask(0xFF);

}

/*
Draw Call 本质是 CPU 向 GPU 发送的一次 “绘制指令” —— 告诉 GPU：“用当前绑定的所有状态（Shader、纹理、顶点缓冲区、渲染状态等），绘制这批顶点 / 图元”。

核心触发源：切换 VAO/VBO、纹理、Shader、FBO、渲染状态（深度 / 混合 / 剔除）是触发新 Draw Call 的最主要原因；
优化核心思路：尽量让更多物体共享 “完全相同的渲染状态”（同 Shader、同纹理图集、同 VAO），通过批处理 / 实例化合并 Draw Call；
关键原则：Draw Call 的本质是 “GPU 绘制状态的一次完整绑定 + 绘制”，只要绑定的状态变了，就会生成新的 Draw Call。  
*/


void init() {
    // 1. 启用面剔除功能（核心开关）
    glEnable(GL_CULL_FACE);

    // 2. 设置要剔除的面（可选，默认是GL_BACK）
    // 可选值：GL_BACK（剔除反面，最常用）、GL_FRONT（剔除正面）、GL_FRONT_AND_BACK（剔除所有面）
    glCullFace(GL_BACK);

    // 3. （可选）修改正面的判断规则（默认是GL_CCW逆时针）
    // glFrontFace(GL_CW);  // 将顺时针设为正面（按需修改）

    // 其他初始化
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}



/*
 glDepthMask 与 glDepthFunc/GL_DEPTH_TEST 的关系
 
函数 / 状态			核心作用									关系
GL_DEPTH_TEST		开关：是否启用深度测试（读取深度缓冲区）		基础，无此开关，glDepthMask/glDepthFunc 均无意义
glDepthFunc(mode)	规则：深度测试的判断条件（如 GL_LESS）		决定「哪些像素能通过测试」，通过后是否写入由 glDepthMask 控制
glDepthMask(mode)	权限：通过测试的像素是否写入深度缓冲区			不影响测试规则，只控制「写入行为」，GL_FALSE 时测试仍生效但不写缓冲区

glDepthMask 用于控制深度缓冲区的写入权限，GL_TRUE（默认）允许写入，GL_FALSE 禁止写入（仅读取）；
核心使用场景是半透明物体渲染：渲染半透明物体时设为 GL_FALSE，避免覆盖深度值，渲染完成后需恢复为 GL_TRUE；
它不影响深度测试的执行（仍会读取深度缓冲区），仅控制测试通过后的写入行为，需与 GL_DEPTH_TEST 配合使用。
 
*/
void init() {
    // 1. 启用深度测试（核心开关）
    glEnable(GL_DEPTH_TEST);
    
    // 2. （可选）设置深度测试规则（默认是 GL_LESS）
    // 规则：只有新像素的深度值 < 缓冲区中已有值时，才通过测试并渲染
    glDepthFunc(GL_LESS);
	/*
	设置深度测试的判断规则（默认 GL_LESS），常用值：
		- GL_LESS：新深度 < 已有深度 → 通过（最常用）
		- GL_GREATER：新深度 > 已有深度 → 通过
		- GL_EQUAL：新深度 = 已有深度 → 通过
		- GL_ALWAYS：始终通过（等同于关闭深度测试）
		- GL_NEVER：始终不通过
	*/



    // 3. 启用面剔除（配合深度测试，优化效果）
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // 初始化背景色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}


{

// 初始化函数
void init() {
    // 启用深度测试（必须）
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // 近的遮挡远的
    glEnable(GL_CULL_FACE); // 配合面剔除优化性能
    glCullFace(GL_BACK);

    // 启用混合（渲染半透明物体需要）
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 标准混合公式

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

// 绘制函数
void display() {
    // 清空颜色和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 调整视角
    gluLookAt(3.0f, 3.0f, 5.0f,  // 摄像机位置
              0.0f, 0.0f, 0.0f,  // 看向原点
              0.0f, 1.0f, 0.0f); // 头顶方向

    // ========== 第一步：渲染不透明物体（红色立方体） ==========
    glDepthMask(GL_TRUE); // 允许写入深度缓冲区（默认，可省略）
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f); // 不透明红色（alpha=1）
    glutSolidCube(1.5); // 绘制不透明立方体

    // ========== 第二步：渲染半透明物体（蓝色立方体） ==========
    glDepthMask(GL_FALSE); // 禁止写入深度缓冲区！
    glColor4f(0.0f, 0.0f, 1.0f, 0.5f); // 半透明蓝色（alpha=0.5）
    glTranslatef(1.0f, 0.0f, 0.0f); // 向右偏移，部分重叠
    glutSolidCube(1.5); // 绘制半透明立方体

    // ========== 恢复默认状态（重要） ==========
    glDepthMask(GL_TRUE); // 恢复深度写入，避免影响后续渲染

    glutSwapBuffers();
}
	
}




{
#include <GL/glut.h>

// 初始化函数
void init() {
    // 1. 启用模板测试（核心开关）
    glEnable(GL_STENCIL_TEST);
    // 2. 启用深度测试（配合使用，避免层级错乱）
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // 3. 启用面剔除（优化性能）
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // 模板缓冲区默认值设为0
    glClearStencil(0);
    // 背景色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

// 绘制函数
void display() {
    // 清空颜色、深度、模板缓冲区（缺一不可）
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    // 调整视角
    gluLookAt(3.0f, 3.0f, 5.0f,  // 摄像机位置
              0.0f, 0.0f, 0.0f,  // 看向原点
              0.0f, 1.0f, 0.0f); // 头顶方向

    // ========== 第一步：绘制物体，标记模板缓冲区 ==========
    // 模板测试规则：始终通过（GL_ALWAYS），参考值为1
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // 第三个参数是掩码（0xFF表示全匹配）
    // 模板操作规则：测试通过时，将模板值设为参考值（1）
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // 允许写入模板缓冲区（默认GL_TRUE，可省略）
    glStencilMask(0xFF);

    // 绘制红色立方体（仅用于标记模板缓冲区，后续会被覆盖）
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(1.5);

    // ========== 第二步：绘制轮廓（仅渲染模板缓冲区非1的像素） ==========
    // 模板测试规则：只有模板值 != 1 时才通过（GL_NOTEQUAL）
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // 模板操作规则：不修改模板缓冲区（全部GL_KEEP）
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    // 禁止写入模板缓冲区（避免修改已标记的模板值）
    glStencilMask(0x00);

    // 禁用深度写入（避免轮廓覆盖物体的深度值）
    glDepthMask(GL_FALSE);

    // 绘制放大的黑色立方体（仅边缘会显示，形成轮廓）
    glColor3f(0.0f, 0.0f, 0.0f);
    glScalef(1.1f, 1.1f, 1.1f); // 放大1.1倍
    glutSolidCube(1.5);

    // ========== 恢复默认状态（重要） ==========
    glStencilMask(0xFF);    // 恢复模板写入
    glDepthMask(GL_TRUE);   // 恢复深度写入
    glStencilFunc(GL_ALWAYS, 0, 0xFF); // 恢复默认测试规则

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // 关键：必须指定 GLUT_STENCIL（创建模板缓冲区）+ GLUT_DEPTH + GLUT_RGB
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GL_STENCIL_TEST Demo");
    
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
	
}