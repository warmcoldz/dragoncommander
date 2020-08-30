#include "file.h"

#include <shellapi.h>
#include <shlwapi.h>

#include <iostream>
#include <memory>
#include <string>

#include "../exception.h"

namespace dragon_commander {
namespace {

std::wstring NormalizePath(const std::wstring& path) {
  wchar_t buffer[MAX_PATH];
  THROW_WIN32_ERROR(::PathCanonicalizeW(buffer, path.c_str()));
  return buffer;
}

std::wstring GetDirectoryMask(const std::wstring& path) {
  return NormalizePath(path) + L"\\*";
}

FileInfo ConvertToFileInfo(const WIN32_FIND_DATA& findData) {
  FileInfo info;
  info.name = findData.cFileName;
  info.type = findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
                  ? FileType::Dir
                  : FileType::RegularFile;
  return info;
}

}  // namespace


void LaunchFile(const std::wstring& path) {
  SHELLEXECUTEINFOW executeInfo{};
  executeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
  executeInfo.fMask = SEE_MASK_FLAG_NO_UI;
  executeInfo.lpFile = path.c_str();
  executeInfo.nShow = SW_NORMAL;

  THROW_WIN32_ERROR(::ShellExecuteExW(&executeInfo));
}

std::vector<FileInfo> GetFilesFromDirectory(const std::wstring& path) {
  std::vector<FileInfo> fileInfos;

  WIN32_FIND_DATA findData;
  std::shared_ptr<void> traveler(
      ::FindFirstFileW(GetDirectoryMask(path).c_str(), &findData), ::FindClose);

  THROW_WIN32_HANDLE_ERROR(traveler.get());

  fileInfos.push_back(ConvertToFileInfo(findData));
  while (::FindNextFileW(traveler.get(), &findData)) {
    fileInfos.push_back(ConvertToFileInfo(findData));
  }

  return fileInfos;
}


void RemoveFile(const std::wstring& path) {
  LPCWSTR lpFileName = path.c_str();
  BOOL res = DeleteFileW(lpFileName);
}

} // namespace dragon_commander
