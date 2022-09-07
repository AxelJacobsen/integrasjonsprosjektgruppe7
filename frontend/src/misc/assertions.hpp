#pragma once

namespace utils {
	namespace internal {
		inline void assertion_failed(const char* expression, const std::source_location& location, const std::string& message = "") {
			if (!message.empty()) {
				logger::error("{}\n{}\nFile \"{}\", line {}, in {}()", message, expression, location.file_name(), location.line(), location.function_name());
			} else {
				logger::error("{}\nFile \"{}\", line {}, in {}()", expression, location.file_name(), location.line(), location.function_name());
			}
			_exit(EXIT_FAILURE);
		}
	}
}

#define VERIFY(expr, ...)                                                                       \
do {                                                                                            \
	if (!static_cast<bool>(expr)) {                                                             \
		utils::internal::assertion_failed(#expr, std::source_location::current(), __VA_ARGS__); \
	}                                                                                           \
} while(0)

#define VERIFY_NOT_REACHED() assertion_failed("not reached", std::source_location_current())

#define TODO() assertion_failed("TODO", std::source_location_current());