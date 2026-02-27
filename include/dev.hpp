/**
 * @file dev.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Brief description
 * @version 0.1.0
 * @date 2026-02-27
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

#include <array>
#include <string_view>

#define DEV_VERSION_STR "@PROJECT_VERSION@"
#define DEV_AUTHOR "@PROJECT_AUTHOR@"
#define DEV_EMAIL "@PROJECT_EMAIL@"
#define DEV_REPOSITORY "@PROJECT_REPOSITORY@"

namespace dev {

constexpr std::string_view version = DEV_VERSION_STR;
constexpr std::string_view author = DEV_AUTHOR;
constexpr std::string_view email = DEV_EMAIL;
constexpr std::string_view repository = DEV_REPOSITORY;

constexpr std::array<std::string_view, 2> username_contributors = {
	"zuudevs",
	"zuu-inner"
};

constexpr std::array<std::string_view, 2> email_contributors = {
	"zuudevs@gmail.com",
	"rafizuhayr001@gmail.com"
};

} // namespace dev