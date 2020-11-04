//==============================================================================
/**
@file       HelloWorldHelper.h

@brief		Example hello world text generator

@copyright  (c) 2020, Zongyi Yang.

**/
//==============================================================================

#pragma once
#include <string>

class HelloWorldHelper
{
public:
	std::string genText();
private:
	const std::string mMsg = "Hello World";
	uint32_t mIdx = 0;
};
