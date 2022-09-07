#pragma once
#include "bash_colors.hpp"

// TODO: log to file?
namespace logger {
	namespace internal {
		// https://bw.org/2021/08/09/cpp20-print/
		template<typename... Args>
		void print(const std::string_view str_fmt, Args&&... args) {
			fputs(std::vformat(str_fmt, std::make_format_args(args...)).c_str(), stdout);
		}
	}

	template <typename... Args>
	void warning(const std::string_view str_fmt, Args&&... args) { 
		const auto formatted = std::vformat(str_fmt, std::make_format_args(args...));
		internal::print("{}[WARN]{} {}\n", colors::YELLOW, colors::RESET, formatted);
	}

	template <typename... Args>
	void error(const std::string_view str_fmt, Args&&... args) { 
		const auto formatted = std::vformat(str_fmt, std::make_format_args(args...));
		internal::print("{}[ERR]{} {}\n", colors::RED, colors::RESET, formatted);
	}

	template <typename... Args>
	void info(const std::string_view str_fmt, Args&&... args) { 
		const auto formatted = std::vformat(str_fmt, std::make_format_args(args...));
		internal::print("{}[INF]{} {}\n", colors::LIGHT_BLUE, colors::RESET, formatted);
	}
}
