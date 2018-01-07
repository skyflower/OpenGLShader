# OpenGLShader 包含opengl库，glew库，fbx解析库，soil图像库
基础版本，包括矩阵(右承)向量基本操作，shader类，纹理类等
1. CTexture纹理类，暂时只有二维纹理和三维纹理， ，一维纹理以及立体纹理没有实现
2. CVertex,CMatrix向量和矩阵类，向量类包括常用向量运算，矩阵类，包括常用乘法逆运算操作，变换矩阵是opengl右乘
3. CObjModel纯基类，用来表示渲染的基类，C3DModel三维渲染模型基类，包括旋转平移缩放等操作。
