#ifndef KRYOGENIC_FOUNDATION_LOG_HPP
#define KRYOGENIC_FOUNDATION_LOG_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Kryogenic/Foundation/Types.hpp"

namespace Kryogenic {
	struct Log final {
		constexpr Log() noexcept  = delete;
		constexpr ~Log() noexcept = delete;

		Log(Log const& pOther)                        = delete;
		Log(Log&& pOther) noexcept                    = delete;
		auto operator=(Log const& pOther) -> Log&     = delete;
		auto operator=(Log&& pOther) noexcept -> Log& = delete;

		auto static SetLevel(spdlog::level::level_enum const pLevel) -> void {
			spdlog::set_level(pLevel);
		}

		auto static Flush() -> void {
			spdlog::flush_on(spdlog::level::trace);
		}

		auto static Debug(cstring const pMessage) -> void {
			spdlog::debug(pMessage);
		}

		auto static Trace(cstring const pMessage) -> void {
			spdlog::trace(pMessage);
		}

		auto static Info(cstring const pMessage) -> void {
			spdlog::info(pMessage);
		}

		auto static Warn(cstring const pMessage) -> void {
			spdlog::warn(pMessage);
		}

		auto static Error(cstring const pMessage) -> void {
			spdlog::error(pMessage);
		}

		auto static Critical(cstring const pMessage) -> void {
			spdlog::critical(pMessage);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static Debug(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::debug(pFormat, std::forward<ARGS>(pArgs)...);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static Trace(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::trace(pFormat, std::forward<ARGS>(pArgs)...);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static Info(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::info(pFormat, std::forward<ARGS>(pArgs)...);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static Warn(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::warn(pFormat, std::forward<ARGS>(pArgs)...);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static Error(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::error(pFormat, std::forward<ARGS>(pArgs)...);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static Critical(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::critical(pFormat, std::forward<ARGS>(pArgs)...);
		}
	};
} // kryogenic

#endif //KRYOGENIC_FOUNDATION_LOG_HPP
