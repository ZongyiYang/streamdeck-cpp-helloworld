//==============================================================================
/**
@file       HelloWorldHelper.h

@brief		Example helper function to mirror Elgato's demo

@copyright  (c) 2020, Zongyi Yang.

**/
//==============================================================================

#pragma once
#include <string>

class HelloWorldHelper
{
public:
	std::string genText(const std::string& msg, const uint32_t idx);
};
