#include "ADFileNameUTF8.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
#include "cocos2d.h"
#include "CCWinRTUtils.h"
const std::wstring ensureUtf8FileName(const std::string& f)
{
	return cocos2d::CCUtf8ToUnicode(f.c_str(), -1);
}
#endif