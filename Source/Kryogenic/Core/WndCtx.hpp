#ifndef KRYOGENIC_CORE_WND_CTX_HPP
#define KRYOGENIC_CORE_WND_CTX_HPP

#include "Kryogenic/Core/Services.hpp"
#include "Kryogenic/Foundation/Types.hpp"

namespace Kryogenic::Core {
	class WndCtx final : public Services::Base {
	public:
		struct Desc final {
			cstring mTitle  = {};
			u16     mWidth  = {};
			u16     mHeight = {};
		};

		WndCtx() noexcept = default;
		~WndCtx() noexcept;

		explicit WndCtx(Desc const& pDesc);

		WndCtx(WndCtx const& pOther)                        = delete;
		WndCtx(WndCtx&& pOther) noexcept                    = default;
		auto operator=(WndCtx const& pOther) -> WndCtx&     = delete;
		auto operator=(WndCtx&& pOther) noexcept -> WndCtx& = default;

		auto Update() noexcept -> void;

		[[nodiscard]] auto GetTitle() const -> cstring;
		[[nodiscard]] auto GetWidth() const -> u16;
		[[nodiscard]] auto GetHeight() const -> u16;
		[[nodiscard]] auto IsOpen() const -> b8;
		[[nodiscard]] auto IsMinimized() const -> b8;
		[[nodiscard]] auto IsMaximized() const -> b8;
		[[nodiscard]] auto IsResized() const -> b8;
		[[nodiscard]] auto IsFocused() const -> b8;

	private:
		void*   mHandle = {};
		cstring mTitle  = {};
		u16     mWidth  = {};
		u16     mHeight = {};

		b8 mIsOpen      = {};
		b8 mIsMinimized = {};
		b8 mIsMaximized = {};
		b8 mIsResized   = {};
		b8 mIsFocused   = {};
	};

	auto GetWndCtx() noexcept -> WndCtx&;
}

#endif //KRYOGENIC_CORE_WND_CTX_HPP
