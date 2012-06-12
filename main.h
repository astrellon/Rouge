#pragma once

// Program uses sprintf.
#define _CRT_SECURE_NO_WARNINGS

#define _VERBOSE_LOG 

#include <Windows.h>

#include "GLee\GLee.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdlib.h>

#include <iostream>

using namespace std;

#include "shader.h"

#include "win_system.h"
#include "game_system.h"
#include "rouge_system.h"
#include "mouse_manager.h"
#include "gfx/gfx_engine.h"
#include "engine.h"
#include "game.h"

#include "logger.h"
#include "file_log_listener.h"

#include "event.h"
#include "event_manager.h"
#include "ievent_listener.h"
