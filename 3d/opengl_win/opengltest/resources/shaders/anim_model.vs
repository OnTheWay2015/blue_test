// 声明GLSL版本为330 core，对应OpenGL 3.3核心配置文件
// 核心配置文件会禁用过时的固定功能管线特性，仅保留可编程管线功能
#version 330 core

// -------------------------- 顶点输入属性（从CPU传递到GPU的顶点数据） --------------------------
// layout(location = 0)：指定顶点属性的位置索引，方便CPU端绑定顶点属性指针
// vec3 pos：顶点位置坐标（三维向量）
layout(location = 0) in vec3 pos;

// vec3 norm：顶点法向量（用于光照计算等，三维向量）
layout(location = 1) in vec3 norm;

// vec2 tex：顶点纹理坐标（UV坐标，二维向量，用于纹理采样）
layout(location = 2) in vec2 tex;

// vec3 tangent：顶点切线向量（用于法线贴图/凹凸贴图，构建TBN坐标系）
layout(location = 3) in vec3 tangent;

// vec3 bitangent：顶点副切线向量（与切线、法向量正交，共同构成TBN坐标系）
layout(location = 4) in vec3 bitangent;

// ivec4 boneIds：影响当前顶点的骨骼ID数组（整型四维向量，最多存储4个骨骼ID）
layout(location = 5) in ivec4 boneIds; 

// vec4 weights：对应boneIds的骨骼权重（四维向量，权重和通常为1.0，控制骨骼对顶点的影响程度）
layout(location = 6) in vec4 weights;

// -------------------------- 统一变量（全局常量，所有顶点共享该值，由CPU端传入） --------------------------
// mat4 projection：投影矩阵（用于将视锥体中的顶点转换为裁剪空间坐标）
uniform mat4 projection;

// mat4 view：视图矩阵（用于将世界空间顶点转换为观察空间（相机空间）坐标）
uniform mat4 view;

// mat4 model：模型矩阵（用于将局部空间顶点转换为世界空间坐标）
uniform mat4 model;

// -------------------------- 骨骼动画相关常量定义 --------------------------
// const int MAX_BONES：场景中最大骨骼数量（常量，不可在运行时修改）
const int MAX_BONES = 100;

// const int MAX_BONE_INFLUENCE：单个顶点最多受多少个骨骼影响（通常为4，平衡性能与效果）
const int MAX_BONE_INFLUENCE = 4;

// uniform mat4 finalBonesMatrices[MAX_BONES]：最终骨骼变换矩阵数组
// 每个元素是骨骼的最终变换矩阵（已结合骨骼的绑定姿势、动画变换等），用于顶点蒙皮计算
uniform mat4 finalBonesMatrices[MAX_BONES];

// -------------------------- 输出变量（传递到片段着色器的数据） --------------------------
// out vec2 TexCoords：输出的纹理坐标，将插值后传递给片段着色器用于纹理采样
out vec2 TexCoords;

// -------------------------- 顶点着色器主函数（核心执行逻辑，每个顶点执行一次） --------------------------
void main()
{
    // 初始化顶点最终局部位置为零向量，用于累加骨骼对顶点的变换影响
    vec4 totalPosition = vec4(0.0f);
    
    // 遍历每个可能影响当前顶点的骨骼（最多MAX_BONE_INFLUENCE个，即4个）
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        // 如果当前骨骼ID为-1，说明该骨骼槽位无效，跳过本次循环
        if(boneIds[i] == -1) 
            continue;
        
        // 安全检查：如果骨骼ID超过最大骨骼数量，说明数据异常
        // 直接使用原始顶点位置，跳出循环不再进行骨骼蒙皮计算
        if(boneIds[i] >= MAX_BONES) 
        {
            totalPosition = vec4(pos, 1.0f); // 将原始顶点位置转换为齐次坐标（w=1.0）
            break; // 跳出for循环，终止骨骼蒙皮计算
        }
        
        // 骨骼蒙皮核心计算：
        // 1. 将原始顶点位置转换为齐次坐标，乘以当前骨骼的最终变换矩阵，得到该骨骼影响下的顶点局部位置
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(pos, 1.0f);
		
        // 2. 将该骨骼影响的顶点位置乘以对应权重，累加到总位置中（权重控制骨骼影响程度）
        totalPosition += localPosition * weights[i];
        
        // （注：此处计算了顶点法向量的骨骼变换，但未存储和使用，可根据光照需求补充输出）
        // 将原始法向量通过骨骼变换矩阵的线性部分（mat3提取前3x3矩阵）进行变换，得到该骨骼影响下的法向量
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * norm;
    }
	
    // 计算视图模型矩阵（view * model）：先将顶点从局部空间转世界空间，再转观察空间
    // 注意：矩阵乘法顺序不可颠倒（GLSL中矩阵为列主序，变换顺序从右往左）
    mat4 viewModel = view * model;
    
    // 设置顶点裁剪空间坐标（gl_Position是顶点着色器的内置输出变量，必须赋值）
    // 变换流程：局部空间（蒙皮后）→ 世界空间 → 观察空间 → 裁剪空间
    gl_Position =  projection * viewModel * totalPosition;
    
    // 将顶点纹理坐标赋值给输出变量，传递给片段着色器（会在图元内部进行插值）
	TexCoords = tex;
}