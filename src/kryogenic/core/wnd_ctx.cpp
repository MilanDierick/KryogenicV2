#include "wnd_ctx.hpp"

#include <SDL2/SDL.h>

#include "kryogenic/foundation/assert.hpp"
#include "kryogenic/foundation/log.hpp"

namespace kryogenic::core {
	wnd_ctx::~wnd_ctx() {
		SDL_DestroyWindow(static_cast<SDL_Window*>(m_handle));
		SDL_Quit();
	}

	wnd_ctx::wnd_ctx(desc const& p_desc)
		: m_title(p_desc.title)
		, m_width(p_desc.width)
		, m_height(p_desc.height) {
		assert_not_null(m_title, "Window title must not be null.");
		assert_not_zero(m_width, "Window width must not be zero.");
		assert_not_zero(m_height, "Window height must not be zero.");

		assert_true(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Failed to initialize SDL2.");

		m_handle = SDL_CreateWindow(
			m_title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_width,
			m_height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
			);

		assert_not_null(m_handle, "Failed to create SDL2 window.");
		m_open = true;
		log::trace("Window created with Title: {} Width: {} Height: {}.", m_title, m_width, m_height);
	}

	auto wnd_ctx::update() noexcept -> void {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					m_open = false;
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_CLOSE:
							m_open = false;
							break;
						case SDL_WINDOWEVENT_RESIZED:
							m_width = static_cast<u16>(event.window.data1);
							m_height  = static_cast<u16>(event.window.data2);
							m_resized = true;
							break;
						case SDL_WINDOWEVENT_MINIMIZED:
							m_minimized = true;
							break;
						case SDL_WINDOWEVENT_MAXIMIZED:
							m_maximized = true;
							break;
						case SDL_WINDOWEVENT_RESTORED:
							m_minimized = false;
							m_maximized = false;
							break;
						case SDL_WINDOWEVENT_FOCUS_GAINED:
							m_focused = true;
							break;
						case SDL_WINDOWEVENT_FOCUS_LOST:
							m_focused = false;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
	}

	auto wnd_ctx::get_title() const -> cstring {
		return m_title;
	}

	auto wnd_ctx::get_width() const -> u16 {
		return m_width;
	}

	auto wnd_ctx::get_height() const -> u16 {
		return m_height;
	}

	auto wnd_ctx::is_open() const -> b8 {
		return m_open;
	}

	auto wnd_ctx::is_minimized() const -> b8 {
		return m_minimized;
	}

	auto wnd_ctx::is_maximized() const -> b8 {
		return m_maximized;
	}

	auto wnd_ctx::is_resized() const -> b8 {
		return m_resized;
	}

	auto wnd_ctx::is_focused() const -> b8 {
		return m_focused;
	}

	auto get_wnd_ctx() noexcept -> wnd_ctx& {
		return services::get<wnd_ctx>();
	}
} // kryogenic::core
