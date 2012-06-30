// My header made for shader.c by Josh A. Beam
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "GLee\GLee.h"
#include <GL/gl.h>

#define blockSize 512

static char *shaderLoadSource(const char *filePath);
static GLuint shaderCompileFromFile(GLenum type, const char *filePath);
void shaderAttachFromFile(GLuint program, GLenum type, const char *filePath);