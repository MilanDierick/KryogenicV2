#include "AppCtx.hpp"

#include "Kryogenic/Core/WndCtx.hpp"

namespace Kryogenic::Core {
	auto AppCtx::Execute() noexcept -> void {
		mIsRunning = true;

		auto& wnd = GetWndCtx();

		while (mIsRunning) {
			wnd.Update();

			if (!wnd.IsOpen()) {
				Terminate();
			}
		}
	}

	auto AppCtx::Terminate() noexcept -> void {
		mIsRunning = false;
	}
}
