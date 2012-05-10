// CSE4GPP Assignment part 1 2011
// Alan Lawrey 15547299

#pragma once

// Program uses sprintf.
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include "GLee\GLee.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "IL\il.h"
#include <stdlib.h>

// Sets that we always have to define the boost::throw_exception function.
//#define BOOST_NO_EXCEPTIONS

//#include <boost\thread\thread.hpp>
//#include <time.h>
#include <iostream>

using namespace std;

//#include "Vector3.hpp"
//#include "Sphere.h"
#include "TextField.h"
#include "shader.h"

#include "NeHeGL.h"

/*
// Size of the room.
//#define ROOM_SIZE 40.0f

// The initial number of spheres that will be added to the scene.
//#define INITIAL_SPHERE_COUNT 3000

// Set the upper and lower limits (although only the lower limit is clamped) 
// for the spring and dampening values so that they may dynamically changed
// to suit the current framerate.
#define UPPER_SPHERE_SPRING_CONST 200.0f
#define LOWER_SPHERE_SPRING_CONST 30.0f

#define UPPER_WALL_SPRING_CONST 800.0f
#define LOWER_WALL_SPRING_CONST 120.0f

#define UPPER_SPHERE_DAMP_FACTOR 2.0f
#define LOWER_SPHERE_DAMP_FACTOR 0.5f

#define UPPER_WALL_DAMP_FACTOR 10.0f
#define LOWER_WALL_DAMP_FACTOR 2.0f

#define UPPER_FRAME_STEP 0.02f
#define LOWER_FRAME_STEP 0.20f

// Linear two point equations for determining the gradient and constant of
// a simple y = mx + c equation.
#define WALL_SPRING_GRAD (LOWER_WALL_SPRING_CONST - UPPER_WALL_SPRING_CONST) / (LOWER_FRAME_STEP - UPPER_FRAME_STEP)
#define WALL_SPRING_CONST WALL_SPRING_GRAD * UPPER_FRAME_STEP + UPPER_WALL_SPRING_CONST

#define SPHERE_SPRING_GRAD (LOWER_SPHERE_SPRING_CONST - UPPER_SPHERE_SPRING_CONST) / (LOWER_FRAME_STEP - UPPER_FRAME_STEP)
#define SPHERE_SPRING_CONST SPHERE_SPRING_GRAD * UPPER_FRAME_STEP + UPPER_SPHERE_SPRING_CONST

#define WALL_DAMP_GRAD (LOWER_WALL_DAMP_FACTOR - UPPER_WALL_DAMP_FACTOR) / (LOWER_FRAME_STEP - UPPER_FRAME_STEP)
#define WALL_DAMP_CONST WALL_DAMP_GRAD * UPPER_FRAME_STEP + UPPER_WALL_DAMP_FACTOR

#define SPHERE_DAMP_GRAD (LOWER_SPHERE_DAMP_FACTOR - UPPER_SPHERE_DAMP_FACTOR) / (LOWER_FRAME_STEP - UPPER_FRAME_STEP)
#define SPHERE_DAMP_CONST SPHERE_DAMP_GRAD * UPPER_FRAME_STEP + UPPER_SPHERE_DAMP_FACTOR

// The velocity at which the user sphere moves when being controlled by the user.
#define USER_SPHERE_SPEED 20.0f

// Sets the maximum number of spheres that are rendered in a given instanced render call.
// Must be a multiple of 3 so the shader can appropriately set the colour and size
// of a sphere.
#define MAX_OBJECTS_PER_RENDER 249

// Defines gravity to by down by 9.8 meters/s^2.
#define GRAVITY Vector3(0.0f, -9.8f, 0.0f)

// Deals with the problem that this function is not defined in Release mode.
void boost::throw_exception(const std::exception &e)
{
}

// Returns a random float between 0.0 and 1.0.
float randf()
{
	return (float)rand() / (float)RAND_MAX;
}

#if defined (_MSC_VER) || defined (__INTEL_COMPILER)
#	include<intrin.h>

void cpuid(int iType, int iOutput[4])
{
	__cpuid(iOutput, iType);
}
#elif defined(__GNUC__) || defined(__SUNPRO_C)
void cpuid(int iType, int iOutput[4])
{
	__asm__ __volatile__ ("cpuid": "=a" (iOutput[0]), "=b" (iOutput[1]),
		"=c" (iOutput[2]), "=d" (iOutput[3]) : "a" (iType) );
}
#else
#	error Unrecognized compiler
#endif

*/