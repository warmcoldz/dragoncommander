#pragma once

#include <Windows.h>

#include <string>
#include <vector>

enum class FileType { Dir, RegularFile };

struct FileInfo {
  std::wstring name;
  FileType type;
};

void LaunchFile(const std::wstring& path);
std::vector<FileInfo> GetFilesFromDirectory(const std::wstring& path);
void RemoveFile(const std::wstring& path);
