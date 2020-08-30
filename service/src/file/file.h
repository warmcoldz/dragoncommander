#pragma once

#include <windows.h>

#include <string>
#include <vector>

namespace dragon_commander {

enum class FileType { Dir, RegularFile };

struct FileInfo {
  std::wstring name;
  FileType type;
};

void LaunchFile(const std::wstring& path);
std::vector<FileInfo> GetFilesFromDirectory(const std::wstring& path);
void RemoveFile(const std::wstring& path);

} // namespace dragon_commander
