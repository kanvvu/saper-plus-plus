#include "FileUtils.hpp"
#include <iostream>

std::vector<std::string> FileUtils::getDirs(const std::string& path) {
	std::vector<std::string> result;

	// Nie DATAA to nie literowka. Niestety.
	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA((path + "*").c_str(), &findData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				result.push_back(findData.cFileName);
				//std::cout << findData.cFileName << std::endl;
			}
		} while (FindNextFileA(hFind, &findData));
	}
	FindClose(hFind);
	return result;
}

// Skanuje takze subdiry!
std::vector<std::string> FileUtils::getAllFiles(const std::string& path, const std::string& extension) {
	std::vector<std::string> result;

	// Nie DATAA to nie literowka. Niestety.
	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA((path + "*" + extension).c_str(), &findData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				result.push_back(path + findData.cFileName);
			}
		} while (FindNextFileA(hFind, &findData));
	}
	FindClose(hFind);
	return result;
}

std::string FileUtils::getFileName(const std::string& path, bool withExtension) {
	int start = path.find_last_of("/\\") + 1;

	if (withExtension)
		return path.substr(start);
	else {
		int end = path.find_last_of(".");

		if (end == std::string::npos)
			return path.substr(start);
		else
			return path.substr(start, end - start);
	}
}