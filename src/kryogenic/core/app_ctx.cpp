#include "app_ctx.hpp"

#include "kryogenic/core/wnd_ctx.hpp"

namespace kryogenic::core {
	auto app_ctx::execute() noexcept -> void {
		m_is_running = true;

		auto& wnd = get_wnd_ctx();

		while (m_is_running) {
			wnd.update();

			if (!wnd.is_open()) {
				terminate();
			}
		}
	}

	auto app_ctx::terminate() noexcept -> void {
		m_is_running = false;
	}
}
