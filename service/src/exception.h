#pragma once

#include <windows.h>

#include <exception>
#include <string>

#include "string_algorithms.h"

namespace dragon_commander {

inline std::string GetLastErrorString() {
  const DWORD err = GetLastError();
  LPVOID lpMsgBuf;
  ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                   nullptr, err, 0, reinterpret_cast<LPTSTR>(&lpMsgBuf), 0,
                   nullptr);

  const std::wstring str = (LPTSTR)lpMsgBuf;
  LocalFree(lpMsgBuf);
  return FromUtf16ToUtf8(str);
}

#define THROW_WIN32_ERROR(expr) \
  if (!(expr)) throw std::runtime_error(GetLastErrorString());
#define THROW_WIN32_HANDLE_ERROR(expr) \
  if ((expr) == INVALID_HANDLE_VALUE)  \
    throw std::runtime_error(GetLastErrorString());

} // namespace dragon_commander
