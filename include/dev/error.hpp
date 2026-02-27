/**
 * @file error.hpp
 * @brief Exit / error codes used by the dispatcher.
 */

#pragma once

namespace dev {

enum class Error : int {
  None = 0,
  General = 1,
  InvalidUsage = 2,
  PermissionDenied = 126,
  CommandNotFound = 127,
};

} // namespace dev
