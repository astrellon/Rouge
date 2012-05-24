#pragma once

// Program uses sprintf.
#define _CRT_SECURE_NO_WARNINGS

#define _VERBOSE_LOG 

#include <Windows.h>

#include "GLee\GLee.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "IL\il.h"
#include <stdlib.h>

#include <iostream>

using namespace std;

#include "shader.h"

#include "win_system.h"
#include "game_system.h"

#include "gl_text_field.h"
#include "gl_gfx_engine.h"
#include "engine.h"

#include "logger.h"
#include "file_log_listener.h"