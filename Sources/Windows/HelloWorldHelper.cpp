//==============================================================================
/**
@file       HelloWorldHelper.cpp

@brief		Example hello world text generator

@copyright  (c) 2020, Zongyi Yang.

**/
//==============================================================================

#include "pch.h"
#include "HelloWorldHelper.h"

std::string HelloWorldHelper::genText()
{
	std::string text = mMsg.substr(0, mIdx);
	mIdx++;
	if (mIdx > mMsg.length())
		mIdx = 0;
	return text;
}