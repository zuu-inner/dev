/**
 * @file command.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Brief description
 * @version 0.1.0
 * @date 2026-02-27
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

#include "config/project.hpp"
#include <string_view>
#include <vector>

namespace dev {
namespace cli {

#if DEV_VERSION >= 0x00000100

struct Command {
	std::string_view name;
	void(*callback)(int, char**);
};

static std::vector<Command> g_commands;

#endif

} // namespace cli
} // namespace dev