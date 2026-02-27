/**
 * @file system.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Brief description
 * @version 0.1.0
 * @date 2026-02-27
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

#include <filesystem>
#include <string_view>

namespace dev::utils {

class System {
	namespace fs = std::filesystem;

	static void create_file(std::string_view path, std::string_view content) noexcept {
		fs::path path_(path);
		if(fs::relative(path_, fs::current_path().)) {

		}
	}
};

} // namespace dev::utils