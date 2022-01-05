#pragma once

/*
	Core definitions for Heavy Engine
	Every key macro or function should be placed here
*/

// Disables enum class warning
#pragma warning(disable: 26812)

// Linker /ignore:4006 removes warning that optick and SFML gave

#define ENABLE_IMGUI		 1
#define DISABLE_PROFILING	 0
#define DISABLE_CONSOLE		 0

#if DISABLE_PROFILING
	#define HV_PROFILE_FRAME(...)
	#define HV_PROFILE_FUNC(...)
	#define HV_PROFILE_TAG(NAME, ...)
	#define HV_PROFILE_THREAD(...)
	#define HV_PROFILE_SCOPE_DYNAMIC(NAME)
	#define HV_LOG(...)
	#define HV_ASSERT(VAR) VAR
	#define HV_DEBUG_ASSERT(VAR)
	#define HV_TIME_FUNC(NAME)

#else
	#include <iostream>
	#include <Optick/optick.h>
	#include "../include/Profile Timer.hpp"

	#define HV_PROFILE_FRAME(...)			OPTICK_FRAME(__VA_ARGS__)
	#define HV_PROFILE_FUNC(...)			OPTICK_EVENT(__VA_ARGS__)
	#define HV_PROFILE_TAG(NAME, ...)		OPTICK_TAG(NAME, __VA_ARGS__)
	#define HV_PROFILE_THREAD(...)			OPTICK_THREAD(__VA_ARGS__)
	#define HV_PROFILE_SCOPE_DYNAMIC(NAME)  OPTICK_EVENT_DYNAMIC(NAME)
	#define HV_LOG(...)						DEBUG_MESSAGE_LOG(__VA_ARGS__)
	#define HV_ASSERT(VAR) if(!(VAR))		__debugbreak();
	#define HV_DEBUG_ASSERT(VAR) if(!(VAR))	__debugbreak();
	#define HV_TIME_FUNC(NAME)				hv::ProfileTimer NAME(#NAME)

	template<typename ...Args>
	constexpr static void DEBUG_MESSAGE_LOG(Args&&... args) 
	{
		(std::cout << ... << args);
	}

#endif

#if DISABLE_CONSOLE
	#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
