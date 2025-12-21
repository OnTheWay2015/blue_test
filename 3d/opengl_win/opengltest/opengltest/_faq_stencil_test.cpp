/*
    片段着色器处理后，模板值通过模板测试（Stencil Test）决定是否丢弃片段，与颜色值无关。
    模板值不直接影响颜色通道，而是控制片段的可见性。模板测试通过后，片段颜色才写入颜色缓冲 
 
模板值的默认行为
    初始化值‌：模板缓冲（Stencil Buffer）在创建时被清除为0，每个像素的模板值默认为0。
    模板测试‌：片段着色器处理后，模板测试（Stencil Test）会根据模板值（默认0）决定是否丢弃片段。

获取模板值的途径
    1.片段着色器输出‌：通过 gl_FragStencilValue 变量设置模板值（默认未设置时为0）。
        glsl
        void main() {
            gl_FragStencilValue = 1; // 设置模板值为1
        }
    2.模板函数控制‌：通过模板函数（如glStencilFunc）设置模板值的比较条件，但不直接设置值。
        glStencilFunc(GL_EQUAL, 1, 0xFF); // 比较模板值是否等于1
关键点总结
    默认值‌：模板值默认为0，片段着色器未显式设置时保持0。
    设置方式‌：通过 gl_FragStencilValue 在片段着色器中显式设置模板值。
    模板测试‌：模板值用于模板测试，决定片段是否通过测试。


glStencilMask 控制模板缓冲区的写入掩码（0x00禁用写入，0xFF启用写入）。
glStencilFunc 设置模板测试函数（如GL_ALWAYS总是通过，GL_EQUAL值相等时通过）。 结果控制 颜色缓充区. 对 颜色缓充区 的操作 (通过测试则更新, 否保持原样)
glStencilOp 定义模板操作（如GL_REPLACE通过测试时写入参考值）。 根据测试结果，对应 glStencilOp 设置的参数， 对 模板缓充区 操作
     glStencilOp (GLenum fail, GLenum zfail, GLenum zpass); // fail,模板测试失败时操作; zfail,深度测试失败时操作; zpass,深度测试成功时操作;

	 示例 ;   
        glStencilOp(GL_REPLACE, GL_KEEP, GL_REPLACE); //第一个参数是 GL_REPLACE, 失败的时候,交换对比测试值 ( 比如上面 glStencilFunc 是1，所以 模板测试失败时会写入模板值1 )

*/