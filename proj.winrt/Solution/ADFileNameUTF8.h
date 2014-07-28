#ifndef ADFILENAMEUTF8_H
#define ADFILENAMEUTF8_H
#include <string>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
const std::wstring ensureUtf8FileName(const std::string& f);
#else
inline const std::string ensureUtf8FileName(const std::string& f)
{
	return f;
}
#endif

#endif