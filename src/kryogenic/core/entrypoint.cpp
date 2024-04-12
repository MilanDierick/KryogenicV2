#include "kryogenic/core/app_ctx.hpp"
#include "kryogenic/core/wnd_ctx.hpp"

int main() {
	using namespace kryogenic;

	constexpr auto wnd_desc = core::wnd_ctx::desc{
		.title = "Kryogenic",
		.width = 800,
		.height = 600
	};

	core::wnd_ctx wnd(wnd_desc);
	core::app_ctx app;

	core::services::set<core::wnd_ctx>(&wnd);
	core::services::set<core::app_ctx>(&app);

	app.execute();

	return EXIT_SUCCESS;
}
