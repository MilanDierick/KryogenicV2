#ifndef KRYOGENIC_FOUNDATION_TYPES_HPP
#define KRYOGENIC_FOUNDATION_TYPES_HPP

#include <map>

using i8  = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using f32 = float;
using f64 = double;

using b8  = bool;
using b16 = bool;
using b32 = bool;
using b64 = bool;

using isize = i64;
using usize = u64;
using byte  = u8;

static_assert(sizeof(i8) == 1, "i8 is not 1 byte.");    // NOLINT(*-magic-numbers)
static_assert(sizeof(i16) == 2, "i16 is not 2 bytes."); // NOLINT(*-magic-numbers)
static_assert(sizeof(i32) == 4, "i32 is not 4 bytes."); // NOLINT(*-magic-numbers)
static_assert(sizeof(i64) == 8, "i64 is not 8 bytes."); // NOLINT(*-magic-numbers)

static_assert(sizeof(u8) == 1, "u8 is not 1 byte.");    // NOLINT(*-magic-numbers)
static_assert(sizeof(u16) == 2, "u16 is not 2 bytes."); // NOLINT(*-magic-numbers)
static_assert(sizeof(u32) == 4, "u32 is not 4 bytes."); // NOLINT(*-magic-numbers)
static_assert(sizeof(u64) == 8, "u64 is not 8 bytes."); // NOLINT(*-magic-numbers)

static_assert(sizeof(f32) == 4, "f32 is not 4 bytes."); // NOLINT(*-magic-numbers)
static_assert(sizeof(f64) == 8, "f64 is not 8 bytes."); // NOLINT(*-magic-numbers)

static_assert(sizeof(b8) == 1, "b8 is not 1 byte.");   // NOLINT(*-magic-numbers)
static_assert(sizeof(b16) == 1, "b16 is not 1 byte."); // NOLINT(*-magic-numbers)
static_assert(sizeof(b32) == 1, "b32 is not 1 byte."); // NOLINT(*-magic-numbers)
static_assert(sizeof(b64) == 1, "b64 is not 1 byte."); // NOLINT(*-magic-numbers)

static_assert(sizeof(isize) == 8, "isize is not 8 bytes."); // NOLINT(*-magic-numbers)
static_assert(sizeof(usize) == 8, "usize is not 8 bytes."); // NOLINT(*-magic-numbers)

#include <array>
#include <unordered_map>
#include <vector>

namespace Kryogenic {
	using cstring = char const*;

	template<typename T>
	using vector = std::vector<T>;

	template<typename T, usize N>
	using array = std::array<T, N>;

	template<typename K, typename V>
	using unordered_map = std::unordered_map<K, V>;

	template<typename K, typename V>
	using multi_map = std::multimap<K, V>;
} // kryogenic

#endif //KRYOGENIC_FOUNDATION_TYPES_HPP
