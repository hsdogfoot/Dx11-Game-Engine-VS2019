#pragma once

#define __FILENAME__ (strrchr(__FILE__,'\\') + 1) 
#define ErrorLog(message) ErrorLogger::Log(__FILENAME__, __FUNCTION__, __LINE__, message)
#define HRLog(hResult) ErrorLogger::LogHResult(__FILENAME__, __FUNCTION__, __LINE__, hResult);
#define HRAndPathLog(hResult, path) ErrorLogger::LogHResultAndPath(__FILENAME__, __FUNCTION__, __LINE__, hResult, path);

class ErrorLogger final abstract
{
public:
	static void Log(std::string file, std::string function, int line, std::string message);
	static void LogHResult(std::string file, std::string function, int line, HRESULT hResult);
	static void LogHResultAndPath(std::string file, std::string function, int line, HRESULT hResult, std::wstring path);
};