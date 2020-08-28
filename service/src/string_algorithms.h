#pragma once

#include <codecvt>
#include <locale>
#include <string>

inline std::wstring FromUtf8ToUtf16(const std::string& utf8) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.from_bytes(utf8);
}

inline std::string FromUtf16ToUtf8(const std::wstring& utf16) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
  return convert.to_bytes(utf16);
}
