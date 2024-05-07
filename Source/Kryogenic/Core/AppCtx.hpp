#ifndef KRYOGENIC_CORE_APP_CTX_HPP
#define KRYOGENIC_CORE_APP_CTX_HPP

#include "Kryogenic/Core/Services.hpp"
#include "Kryogenic/Foundation/Types.hpp"

namespace Kryogenic::Core {
	class AppCtx final : public Services::Base {
	public:
		AppCtx() noexcept  = default;
		~AppCtx() noexcept = default;

		AppCtx(AppCtx const& pOther)                        = delete;
		AppCtx(AppCtx&& pOther) noexcept                    = default;
		auto operator=(AppCtx const& pOther) -> AppCtx&     = delete;
		auto operator=(AppCtx&& pOther) noexcept -> AppCtx& = default;

		auto Execute() noexcept -> void;
		auto Terminate() noexcept -> void;

	private:
		b8 mIsRunning = {};
	};
} // kryogenic

#endif //KRYOGENIC_CORE_APP_CTX_HPP
