#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void RenderSceneCB();
void InitializeGlutCallbacks();
void glut_initialization ();
void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
void CompileShaders();