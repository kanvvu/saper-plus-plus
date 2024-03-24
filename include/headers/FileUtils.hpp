#pragma once
#include <Windows.h>
#include <string>
#include <vector>

class FileUtils {
public:
	static std::vector<std::string> getDirs(const std::string& path);
	static std::vector<std::string> getAllFiles(const std::string& path, const std::string& extension);
	static std::string getFileName(const std::string& path, bool withExtension = false);
};