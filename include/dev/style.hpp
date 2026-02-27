/**
 * @file style.hpp
 * @brief ANSI terminal colors and style utilities.
 */

#pragma once

#include <cstdio>
#include <string>
#include <string_view>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace dev::style {

// ── ANSI escape sequences ───────────────────────────────────

inline constexpr const char *reset = "\033[0m";
inline constexpr const char *bold = "\033[1m";
inline constexpr const char *dim = "\033[2m";

inline constexpr const char *red = "\033[31m";
inline constexpr const char *green = "\033[32m";
inline constexpr const char *yellow = "\033[33m";
inline constexpr const char *blue = "\033[34m";
inline constexpr const char *cyan = "\033[36m";
inline constexpr const char *gray = "\033[90m";

// ── Detection ───────────────────────────────────────────────

/// Check if stdout is a terminal (not piped/redirected).
inline bool is_tty() {
#ifdef _WIN32
  return _isatty(_fileno(stdout)) != 0;
#else
  return isatty(fileno(stdout)) != 0;
#endif
}

/// Enable Virtual Terminal Processing on Windows.
/// No-op on other platforms.  Call once at startup.
inline void enable_colors() {
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE)
    return;

  DWORD mode = 0;
  if (!GetConsoleMode(hOut, &mode))
    return;

  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, mode);

  // Also enable for stderr
  HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
  if (hErr != INVALID_HANDLE_VALUE) {
    GetConsoleMode(hErr, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hErr, mode);
  }
#endif
}

// ── Global state ────────────────────────────────────────────

/// Whether color output is enabled (set via init()).
inline bool g_colors = false;

/// Initialise color support.  Call once from main().
inline void init() {
  enable_colors();
  g_colors = is_tty();
}

// ── Helpers ─────────────────────────────────────────────────

/// Wrap text in a style.  Returns plain text if colors are off.
inline std::string styled(const char *style_code, std::string_view text) {
  if (!g_colors)
    return std::string(text);
  return std::string(style_code) + std::string(text) + reset;
}

inline std::string bold_text(std::string_view t) { return styled(bold, t); }
inline std::string dim_text(std::string_view t) { return styled(dim, t); }
inline std::string red_text(std::string_view t) { return styled(red, t); }
inline std::string green_text(std::string_view t) { return styled(green, t); }
inline std::string yellow_text(std::string_view t) { return styled(yellow, t); }
inline std::string cyan_text(std::string_view t) { return styled(cyan, t); }
inline std::string gray_text(std::string_view t) { return styled(gray, t); }

} // namespace dev::style
