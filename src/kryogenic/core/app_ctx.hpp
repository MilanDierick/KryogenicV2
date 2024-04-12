#ifndef KRYOGENIC_CORE_APP_CTX_HPP
#define KRYOGENIC_CORE_APP_CTX_HPP

#include "kryogenic/core/services.hpp"
#include "kryogenic/foundation/types.hpp"

namespace kryogenic::core {
	class app_ctx final : public services::base {
	public:
		app_ctx() noexcept  = default;
		~app_ctx() noexcept = default;

		app_ctx(app_ctx const& p_other)                        = delete;
		app_ctx(app_ctx&& p_other) noexcept                    = default;
		auto operator=(app_ctx const& p_other) -> app_ctx&     = delete;
		auto operator=(app_ctx&& p_other) noexcept -> app_ctx& = default;

		auto execute() noexcept -> void;
		auto terminate() noexcept -> void;

	private:
		b8 m_is_running = {};
	};
} // kryogenic

#endif //KRYOGENIC_CORE_APP_CTX_HPP
