#pragma once
#include "../pch.hpp"

namespace utils {
	inline std::string load_file(const std::string_view path) {
		// Open the file and check if it's open
		std::ifstream f(path.data());
		if (!f.is_open()) {
			return "";
		}

		// Read in file data and return it
		return {
			(std::istreambuf_iterator<char>(f)),
			std::istreambuf_iterator<char>()
		};
	}
	}
