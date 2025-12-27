/*
 https://github.com/assimp/assimp

 在3D图形开发中，‌模型（Model）‌通常指包含几何数据（如顶点、索引、法线、纹理坐标）和材质信息的3D对象。‌
ASSIMP（Open Asset Import Library）‌是用于加载和处理多种3D文件格式的开源库，支持‌扩展（Extensions）‌功能，允许开发者自定义文件解析行为。

扩展文件（Extension File）‌
扩展文件是ASSIMP通过特定后缀（如.obj、.fbx、.3ds等）标识的3D模型文件，包含模型的几何和材质数据。ASSIMP支持的扩展文件格式包括但不限于：

.obj（Wavefront OBJ）
		OBJ 文件是一种广泛使用的三维模型文件格式 
		由Wavefront Technologies公司开发，主要用于存储三维物体的几何信息（如顶点、面、纹理坐标等）。
		obj文件是3D模型文件格式。
		由Alias|Wavefront公司为3D建模和动画软件"Advanced Visualizer"开发的一种标准，
		适合用于3D软件模型之间的互导，也可以通过Maya读写。
		不包含动画、材质特性、贴图路径、动力学、粒子等信息。

.fbx（Autodesk FBX）
	Autodesk FBX 是一种由 Autodesk 公司开发的三维文件格式，主要用于跨平台、跨软件的三维数据交换。‌
	它最初是 Autodesk MotionBuilder 的原生格式，用于存储和共享三维模型、动画、场景信息等。‌
	该格式支持丰富的数据类型，包括模型几何体、材质纹理、灯光、摄像机、骨骼动画、关键帧动画以及二维元素（如音频和视频）。‌
	FBX 文件可以采用二进制或 ASCII 文本格式存储，其中二进制格式更高效，而 ASCII 格式便于人类阅读和调试。‌
	FBX 格式被广泛集成到主流三维软件中，如 3ds Max、Maya、Softimage 等，以及游戏引擎（如 Unity 和 Unreal Engine），实现了不同工具之间的互操作性。‌
	它特别适用于游戏开发、影视特效、虚拟现实等领域的数据互通，能够保留复杂的场景层次和动画数据。‌
	此外，Autodesk 提供了 FBX SDK（软件开发工具包），允许开发者自定义数据交互，进一步增强了格式的灵活性。‌
	由于其成熟的支持和兼容性，FBX 成为三维创作中常用的交换格式之一。‌

.3ds（3D Studio Max）
.dae（COLLADA）
.blend（Blender）
.ply（Stanford PLY）
.stl（Stereolithography）
...

*/

