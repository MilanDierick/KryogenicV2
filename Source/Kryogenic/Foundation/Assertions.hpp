#ifndef KRYOGENIC_FOUNDATION_ASSERT_HPP
#define KRYOGENIC_FOUNDATION_ASSERT_HPP

#include <stdexcept>

#include "Kryogenic/Foundation/Types.hpp"

namespace Kryogenic {
	constexpr static auto AssertTrue(b8 const pCondition, cstring const pMessage) -> void {
		if (!pCondition) {
			throw std::runtime_error(pMessage);
		}
	}

	constexpr static auto AssertFalse(b8 const pCondition, cstring const pMessage) -> void {
		if (pCondition) {
			throw std::runtime_error(pMessage);
		}
	}

	constexpr static auto AssertNull(void const* const pPtr, cstring const pMessage) -> void {
		if (nullptr != pPtr) {
			throw std::runtime_error(pMessage);
		}
	}

	constexpr static auto AssertNotNull(void const* const pPtr, cstring const pMessage) -> void {
		if (nullptr == pPtr) {
			throw std::runtime_error(pMessage);
		}
	}

	constexpr static auto AssertNotZero(u64 const pValue, cstring const pMessage) -> void {
		if (0 == pValue) {
			throw std::runtime_error(pMessage);
		}
	}
} // kryogenic

#endif //KRYOGENIC_FOUNDATION_ASSERT_HPP
