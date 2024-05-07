#include "WndCtx.hpp"

#include <SDL2/SDL.h>

#include "Kryogenic/Foundation/Assertions.hpp"
#include "Kryogenic/Foundation/Log.hpp"

namespace Kryogenic::Core {
	WndCtx::~WndCtx() {
		SDL_DestroyWindow(static_cast<SDL_Window*>(mHandle));
		SDL_Quit();
	}

	WndCtx::WndCtx(Desc const& pDesc)
		: mTitle(pDesc.mTitle)
		, mWidth(pDesc.mWidth)
		, mHeight(pDesc.mHeight) {
		AssertNotNull(mTitle, "Window title must not be null.");
		AssertNotZero(mWidth, "Window width must not be zero.");
		AssertNotZero(mHeight, "Window height must not be zero.");

		AssertTrue(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Failed to initialize SDL2.");

		mHandle = SDL_CreateWindow(
			mTitle,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			mWidth,
			mHeight,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
			);

		AssertNotNull(mHandle, "Failed to create SDL2 window.");
		mIsOpen = true;
		Log::Trace("Window created with Title: {} Width: {} Height: {}.", mTitle, mWidth, mHeight);
	}

	auto WndCtx::Update() noexcept -> void {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					mIsOpen = false;
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_CLOSE:
							mIsOpen = false;
							break;
						case SDL_WINDOWEVENT_RESIZED:
							mWidth = static_cast<u16>(event.window.data1);
							mHeight     = static_cast<u16>(event.window.data2);
							mIsResized = true;
							break;
						case SDL_WINDOWEVENT_MINIMIZED:
							mIsMinimized = true;
							break;
						case SDL_WINDOWEVENT_MAXIMIZED:
							mIsMaximized = true;
							break;
						case SDL_WINDOWEVENT_RESTORED:
							mIsMinimized = false;
							mIsMaximized = false;
							break;
						case SDL_WINDOWEVENT_FOCUS_GAINED:
							mIsFocused = true;
							break;
						case SDL_WINDOWEVENT_FOCUS_LOST:
							mIsFocused = false;
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

	auto WndCtx::GetTitle() const -> cstring {
		return mTitle;
	}

	auto WndCtx::GetWidth() const -> u16 {
		return mWidth;
	}

	auto WndCtx::GetHeight() const -> u16 {
		return mHeight;
	}

	auto WndCtx::IsOpen() const -> b8 {
		return mIsOpen;
	}

	auto WndCtx::IsMinimized() const -> b8 {
		return mIsMinimized;
	}

	auto WndCtx::IsMaximized() const -> b8 {
		return mIsMaximized;
	}

	auto WndCtx::IsResized() const -> b8 {
		return mIsResized;
	}

	auto WndCtx::IsFocused() const -> b8 {
		return mIsFocused;
	}

	auto GetWndCtx() noexcept -> WndCtx& {
		return Services::Get<WndCtx>();
	}
} // kryogenic::core
