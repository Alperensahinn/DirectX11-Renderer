#pragma once

#define CHECK_INFOQUEUE(func)\
	infoManager.Set();\
	(func);\
	{\
		auto m = infoManager.GetMessages();\
		if(!m.empty()) \
		{\
			infoManager.PrintMessageQueue(m); \
			DebugBreak();\
		}\
	}