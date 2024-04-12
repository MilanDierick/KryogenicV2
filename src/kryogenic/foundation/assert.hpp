#ifndef KRYOGENIC_FOUNDATION_ASSERT_HPP
#define KRYOGENIC_FOUNDATION_ASSERT_HPP

#include <stdexcept>

#include "kryogenic/foundation/types.hpp"

namespace kryogenic {
	constexpr static auto assert_true(b8 const pCondition, cstring const pMessage) -> void {
		if (!pCondition) {
			throw std::runtime_error(pMessage);
		}
	}

	constexpr static auto assert_false(b8 const pCondition, cstring const pMessage) -> void {
		if (pCondition) {
			throw std::runtime_error(pMessage);
		}
	}

	constexpr static auto assert_null(void const* const pPtr, cstring const pMessage) -> void {
		if (nullptr != pPtr) {
			throw std::runtime_error(pMessage);
		}
	}

	constexpr static auto assert_not_null(void const* const pPtr, cstring const pMessage) -> void {
		if (nullptr == pPtr) {
			throw std::runtime_error(pMessage);
		}
	}

	constexpr static auto assert_not_zero(u64 const pValue, cstring const pMessage) -> void {
		if (0 == pValue) {
			throw std::runtime_error(pMessage);
		}
	}
} // kryogenic

#endif //KRYOGENIC_FOUNDATION_ASSERT_HPP
