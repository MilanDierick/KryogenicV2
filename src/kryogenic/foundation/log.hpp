#ifndef KRYOGENIC_FOUNDATION_LOG_HPP
#define KRYOGENIC_FOUNDATION_LOG_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "kryogenic/foundation/types.hpp"

namespace kryogenic {
	struct log final {
		constexpr log() noexcept  = delete;
		constexpr ~log() noexcept = delete;

		log(log const& pOther)                        = delete;
		log(log&& pOther) noexcept                    = delete;
		auto operator=(log const& pOther) -> log&     = delete;
		auto operator=(log&& pOther) noexcept -> log& = delete;

		auto static set_level(spdlog::level::level_enum const pLevel) -> void {
			spdlog::set_level(pLevel);
		}

		auto static flush() -> void {
			spdlog::flush_on(spdlog::level::trace);
		}

		auto static debug(cstring const pMessage) -> void {
			spdlog::debug(pMessage);
		}

		auto static trace(cstring const pMessage) -> void {
			spdlog::trace(pMessage);
		}

		auto static info(cstring const pMessage) -> void {
			spdlog::info(pMessage);
		}

		auto static warn(cstring const pMessage) -> void {
			spdlog::warn(pMessage);
		}

		auto static error(cstring const pMessage) -> void {
			spdlog::error(pMessage);
		}

		auto static critical(cstring const pMessage) -> void {
			spdlog::critical(pMessage);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static debug(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::debug(pFormat, std::forward<ARGS>(pArgs)...);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static trace(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::trace(pFormat, std::forward<ARGS>(pArgs)...);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static info(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::info(pFormat, std::forward<ARGS>(pArgs)...);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static warn(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::warn(pFormat, std::forward<ARGS>(pArgs)...);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static error(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::error(pFormat, std::forward<ARGS>(pArgs)...);
		}

		template<typename... ARGS> requires (sizeof...(ARGS) != 0)
		auto static critical(spdlog::format_string_t<ARGS...> pFormat, ARGS... pArgs) -> void {
			spdlog::critical(pFormat, std::forward<ARGS>(pArgs)...);
		}
	};
} // kryogenic

#endif //KRYOGENIC_FOUNDATION_LOG_HPP
