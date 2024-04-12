#ifndef KRYOGENIC_FOUNDATION_SINGLETON_HPP
#define KRYOGENIC_FOUNDATION_SINGLETON_HPP

namespace kryogenic {
	template<class T>
	class singleton {
	public:
		constexpr         singleton() noexcept  = default;
		constexpr virtual ~singleton() noexcept = default;

		singleton(singleton const& p_other)                        = delete;
		singleton(singleton&& p_other) noexcept                    = delete;
		auto operator=(singleton const& p_other) -> singleton&     = delete;
		auto operator=(singleton&& p_other) noexcept -> singleton& = delete;

		static auto instance() -> T& {
			static T instance;
			return instance;
		}
	};
}

#endif //KRYOGENIC_FOUNDATION_SINGLETON_HPP
