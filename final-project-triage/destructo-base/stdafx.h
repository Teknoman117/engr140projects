// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef __DERP__
#define __DERP__


//Basic I/O
#include <iostream>

//Handy containers
#include <vector>
#include <set>
#include <map>
#include <list>
#include <string>
#include <array>
#include <queue>

//Other utilities
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cmath>
#include <functional>
#include <limits>
#include <stack>

//for threading
#include <mutex>
#include <thread>

using namespace std;

//Define assert for non windows based platforms
#ifndef _ASSERTE
#define _ASSERTE assert
#endif

//visual studio is retarded
#ifdef WIN32
#define noexcept throw()
#endif

//Too lazy to allow Unicode support (only effects windows)
#ifdef UNICODE
#undef UNICODE
#endif

#if (defined WIN32)
# include <windows.h>
# include <GL/glew.h>
#elif (defined __APPLE__)
# include <GL/glew.h>
# include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

//GLM
#include <glm/glm.hpp>
//#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

//Basic types used everywhere
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::quat;

//Simulation delta
#define SIMULATION_DELTA .01f

//Useful defines
#define ADDSUFFIX_2(x,y) x ## y
#define ADDSUFFIX_1(x,y) ADDSUFFIX_2(x,y)

//These defines are designed to allow the user to make warnings
//in visual studio. They can be ignored
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "(" __STR1__(__LINE__) ") : Warning Msg: "

//Allow packing of structures in GCC and in VisualC++ not sure about clang
#ifdef WIN32
//Assume visual studio
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#else
//Assume GCC (I have no idea how to make this work with clang)
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

// Clamp function
template <typename T>
inline float clamp(T x, T a, T b)
{
    return x < a ? a : (x > b ? b : x);
}

#endif
