#ifndef KRYOGENIC_CORE_WND_CTX_HPP
#define KRYOGENIC_CORE_WND_CTX_HPP

#include "kryogenic/core/services.hpp"
#include "kryogenic/foundation/types.hpp"

namespace kryogenic::core {
	class wnd_ctx final : public services::base {
	public:
		struct desc final {
			cstring title  = {};
			u16     width  = {};
			u16     height = {};
		};

		wnd_ctx() noexcept = default;
		~wnd_ctx() noexcept;

		explicit wnd_ctx(desc const& p_desc);

		wnd_ctx(wnd_ctx const& p_other)                        = delete;
		wnd_ctx(wnd_ctx&& p_other) noexcept                    = default;
		auto operator=(wnd_ctx const& p_other) -> wnd_ctx&     = delete;
		auto operator=(wnd_ctx&& p_other) noexcept -> wnd_ctx& = default;

		auto update() noexcept -> void;

		[[nodiscard]] auto get_title() const -> cstring;
		[[nodiscard]] auto get_width() const -> u16;
		[[nodiscard]] auto get_height() const -> u16;
		[[nodiscard]] auto is_open() const -> b8;
		[[nodiscard]] auto is_minimized() const -> b8;
		[[nodiscard]] auto is_maximized() const -> b8;
		[[nodiscard]] auto is_resized() const -> b8;
		[[nodiscard]] auto is_focused() const -> b8;

	private:
		void*   m_handle = {};
		cstring m_title  = {};
		u16     m_width  = {};
		u16     m_height = {};

		b8 m_open      = {};
		b8 m_minimized = {};
		b8 m_maximized = {};
		b8 m_resized   = {};
		b8 m_focused   = {};
	};

	auto get_wnd_ctx() noexcept -> wnd_ctx&;
}

#endif //KRYOGENIC_CORE_WND_CTX_HPP
