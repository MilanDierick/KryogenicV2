#ifndef KRYOGENIC_FOUNDATION_CONCEPTS_HPP
#define KRYOGENIC_FOUNDATION_CONCEPTS_HPP

namespace Kryogenic {
	template<class T>
	concept DefaultConstructible = std::is_default_constructible_v<T>;

	template<class T>
	concept TriviallyDestructible = std::is_trivially_destructible_v<T>;

	template<class T, class... ARGS>
	concept NothrowConstructible = std::is_nothrow_constructible_v<T, ARGS...>;

	template<class T>
	concept NothrowDestructible = std::is_nothrow_destructible_v<T>;

	template<class T>
	concept NothrowInvocable = std::is_nothrow_invocable_v<T>;

	template<class T, class... ARGS>
	concept ConstructibleFrom = std::is_constructible_v<T, ARGS...>;
}

#endif //KRYOGENIC_FOUNDATION_CONCEPTS_HPP
