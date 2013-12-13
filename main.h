#pragma once

// Program uses sprintf.
#define _CRT_SECURE_NO_WARNINGS

#define _VERBOSE_LOG 

#ifdef _WIN_SYS
#	include <Windows.h>
#	include "GLee\GLee.h"
#	include <gl\gl.h>
#	include <gl\glu.h>
#endif
#include <stdlib.h>   

#include <iostream>

#ifdef _WIN_SYS
#	include <sys/win/win_system.h>
#elif defined _FREEGLUT_SYS
#	include <sys/freeglut/freeglut_system.h>
#endif

#include <sys/game_system.h>
#include <sys/rouge_system.h>
#include <sys/unit_test_system.h>
#include <sys/mem_test_system.h>
#include <ui/mouse_manager.h>
#include <ui/keyboard_manager.h>
#include <gfx/gfx_engine.h>
#include <sfx/sfx_engine.h>
#include <game/engine.h>
#include <game/game.h>

#include <log/logger.h>
#include <log/file_log_listener.h>

#include <ui/event.h>
#include <ui/event_manager.h>
#include <ui/ievent_listener.h>

namespace am {
	int amMain(int argc, char ** argv);
}
