#include "Kryogenic/Core/AppCtx.hpp"
#include "Kryogenic/Core/WndCtx.hpp"
#include "Kryogenic/Ecs/Registry.hpp"
#include "Kryogenic/Foundation/Log.hpp"
#include "Kryogenic/Foundation/Types.hpp"

struct Position final {
	f32 mX = {};
	f32 mY = {};
};

struct Velocity final {
	f32 mX = {};
	f32 mY = {};
};

auto main() -> i32 {
	using namespace Kryogenic;

	Log::SetLevel(spdlog::level::trace);

	constexpr Core::WndCtx::Desc desc{
		.mTitle = "Kryogenic",
		.mWidth = 1280,
		.mHeight = 720,
	};

	Core::AppCtx appCtx;
	Core::WndCtx wndCtx(desc);
	Ecs::Registry registry;

	Core::Services::Set<Core::AppCtx>(&appCtx);
	Core::Services::Set<Core::WndCtx>(&wndCtx);
	Core::Services::Set<Ecs::Registry>(&registry);

	auto const earth = registry.Create("earth");
	registry.Set<Position>(earth, {0.0f, 0.0f});
	registry.Set<Velocity>(earth, {0.0f, 0.0f});

	auto const moon = registry.Create("moon");
	registry.Set<Position>(moon, {1.0f, 1.0f});

	auto const moon2 = registry.Create("moon2");

	registry.Add<Ecs::ChildOf>(moon, earth);
	registry.Add<Ecs::ChildOf>(moon2, earth);

	registry.QueryRelation<Ecs::ChildOf>([&](auto const& pSource, auto const& pTarget) {
		auto const& childName  = registry.Get<std::string>(pSource);
		auto const& parentName = registry.Get<std::string>(pTarget);

		Log::Info("{} is a child of {}", childName, parentName);
	});

	appCtx.Execute();

	return EXIT_SUCCESS;
}
