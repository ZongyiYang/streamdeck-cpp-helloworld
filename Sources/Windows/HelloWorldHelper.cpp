//==============================================================================
/**
@file       HelloWorldHelper.cpp

@brief		Example helper function to mirror Elgato's demo

@copyright  (c) 2020, Zongyi Yang.

**/
//==============================================================================

#include "pch.h"
#include "HelloWorldHelper.h"

std::string HelloWorldHelper::genText(const std::string & msg, const uint32_t idx)
{
	return msg.substr(0, idx);
}