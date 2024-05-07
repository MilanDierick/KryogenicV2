#ifndef KRYOGENIC_ECS_COMP_SET_HPP
#define KRYOGENIC_ECS_COMP_SET_HPP

#include <ranges>

#include "Kryogenic/Foundation/Assertions.hpp"
#include "Kryogenic/Foundation/Concepts.hpp"

namespace Kryogenic::Ecs {
	using EntityId    = u32;
	using Tombstone   = EntityId;
	using ComponentId = EntityId;

	constexpr auto INVALID_ENTITY_ID = std::numeric_limits<EntityId>::max();

	struct DenseContainer final {
		usize mSize     = {}; // number of components
		usize mCapacity = {}; // number of components that can be stored
		usize mStride   = {}; // size of the component in bytes
		byte* mData     = {}; // the actual component data
	};

	class ComponentSet final {
	public:
		ComponentSet() noexcept  = default;
		~ComponentSet() noexcept = default;

		explicit ComponentSet(usize const pStride) noexcept
			: mStorage(std::make_unique<Storage>()) {
			auto& [_, mDense] = *mStorage;

			mDense.mStride = pStride;
		}

		ComponentSet(ComponentSet const& pOther)                        = delete;
		ComponentSet(ComponentSet&& pOther) noexcept                    = default;
		auto operator=(ComponentSet const& pOther) -> ComponentSet&     = delete;
		auto operator=(ComponentSet&& pOther) noexcept -> ComponentSet& = default;

		[[nodiscard]] auto operator[](EntityId const& pEntity) noexcept -> byte* {
			auto const& sparse = mStorage->mSparse;
			auto const& dense  = mStorage->mDense;
			auto const  idx    = sparse[pEntity];
			return dense.mData + idx * dense.mStride;
		}

		[[nodiscard]] auto operator[](EntityId const& pEntity) const noexcept -> byte const* {
			auto const& sparse = mStorage->mSparse;
			auto const& dense  = mStorage->mDense;
			auto const  idx    = sparse[pEntity];
			return dense.mData + idx * dense.mStride;
		}

		[[nodiscard]] auto At(EntityId const& pEntity) noexcept -> byte* {
			auto const& sparse = mStorage->mSparse;
			auto const& dense  = mStorage->mDense;

			if (!Contains(pEntity)) {
				return nullptr;
			}

			auto const idx = sparse[pEntity];
			return dense.mData + idx * dense.mStride;
		}

		[[nodiscard]] auto At(EntityId const& pEntity) const noexcept -> byte const* {
			auto const& sparse = mStorage->mSparse;
			auto const& dense  = mStorage->mDense;

			if (!Contains(pEntity)) {
				return nullptr;
			}

			auto const idx = sparse[pEntity];
			return dense.mData + idx * dense.mStride;
		}

		template<class T>
		[[nodiscard]] auto Get(EntityId const& pEntity) noexcept -> T& {
			auto const& sparse = mStorage->mSparse;
			auto&       dense  = mStorage->mDense;
			auto const  idx    = sparse[pEntity];
			return *reinterpret_cast<T*>(dense.mData + idx * dense.mStride);
		}

		template<class T>
		[[nodiscard]] auto Get(EntityId const& pEntity) const noexcept -> T const& {
			auto const& sparse = mStorage->mSparse;
			auto const& dense  = mStorage->mDense;
			auto const  idx    = sparse[pEntity];
			return *reinterpret_cast<T const*>(dense.mData + idx * dense.mStride);
		}

		[[nodiscard]] auto size() const noexcept -> usize {
			auto const& [sparse, _] = *mStorage;
			return sparse.size();
		}

		[[nodiscard]] auto Capacity() const noexcept -> usize {
			auto const& [sparse, _] = *mStorage;
			return sparse.capacity();
		}

		[[nodiscard]] auto Empty() const noexcept -> b8 {
			auto const& [sparse, _] = *mStorage;
			return sparse.empty();
		}

		[[nodiscard]] auto Contains(EntityId const& pEntity) const noexcept -> b8 {
			auto const& [sparse, _] = *mStorage;
			return pEntity < sparse.size() && sparse[pEntity] != INVALID_ENTITY_ID;
		}

		template<class T>
		[[nodiscard]] auto Insert(EntityId const& pEntity, T const& pValue) noexcept -> T& {
			auto& sparse = mStorage->mSparse;
			auto& dense  = mStorage->mDense;

			AssertTrue(sizeof(T) == dense.mStride, "Component size mismatch.");

			if (Contains(pEntity)) {
				auto const idx = sparse[pEntity];
				T* const   ptr = reinterpret_cast<T*>(&dense.mData[idx * dense.mStride]);
				std::copy_n(reinterpret_cast<byte const*>(&pValue), dense.mStride, reinterpret_cast<byte*>(ptr));
				return *ptr;
			}

			if (sparse.size() <= pEntity) {
				sparse.resize(std::max(sparse.capacity() * 2, usize{16}), INVALID_ENTITY_ID);
			}

			if (dense.mCapacity <= dense.mSize + 1) {
				ResizeDense(std::max(dense.mCapacity * 2, usize{16}));
			}

			auto const idx  = dense.mSize;
			sparse[pEntity] = static_cast<EntityId>(idx);
			dense.mSize += 1;

			T* const ptr = reinterpret_cast<T*>(&dense.mData[idx * dense.mStride]);
			std::construct_at(ptr, pValue);
			return *ptr;
		}

		template<class T, typename... ARGS> requires ConstructibleFrom<T, ARGS...>
		[[nodiscard]] auto Insert(EntityId const& pEntity, ARGS&&... pArgs) noexcept -> T& {
			auto& sparse = mStorage->mSparse;
			auto& dense  = mStorage->mDense;

			AssertTrue(sizeof(T) == dense.mStride, "Component size mismatch.");

			if (Contains(pEntity)) {
				auto const idx = sparse[pEntity];
				T* const   ptr = reinterpret_cast<T*>(&dense.mData[idx * dense.mStride]);
				std::construct_at(ptr, std::forward<ARGS>(pArgs)...);
				return *ptr;
			}

			if (sparse.size() <= pEntity) {
				sparse.resize(std::max(sparse.capacity() * 2, usize{16}), INVALID_ENTITY_ID);
			}

			if (dense.mCapacity <= dense.mSize + 1) {
				ResizeDense(std::max(dense.mCapacity * 2, usize{16}));
			}

			auto const idx  = dense.mSize;
			sparse[pEntity] = static_cast<EntityId>(idx);
			dense.mSize += 1;

			T* const ptr = reinterpret_cast<T*>(&dense.mData[idx * dense.mStride]);
			std::construct_at(ptr, std::forward<ARGS>(pArgs)...);
			return *ptr;
		}

		template<class T>
		auto Assign(EntityId const& pEntity, T const& pValue) noexcept -> T& {
			auto const& sparse = mStorage->mSparse;
			auto&       dense  = mStorage->mDense;

			if (!Contains(pEntity)) {
				return Insert<T>(pEntity, pValue);
			}

			auto const  idx = sparse[pEntity];
			auto* const ptr = reinterpret_cast<T*>(&dense.mData[idx * dense.mStride]);
			*ptr            = pValue;
			return *ptr;
		}

		auto Erase(EntityId const& pEntity) noexcept -> void {
			auto& sparse = mStorage->mSparse;
			auto& dense  = mStorage->mDense;

			if (!Contains(pEntity)) {
				return;
			}

			auto const lastIter = std::ranges::find_if(sparse | std::views::reverse, [](auto const& pId) {
				return pId != INVALID_ENTITY_ID;
			});

			if (lastIter == sparse.rend()) {
				sparse.clear();
				dense.mSize = 0;
				return;
			}

			auto const last    = *lastIter;
			auto const idx     = sparse[pEntity];
			auto const lastIdx = sparse[last];
			auto const offset  = idx * dense.mStride;
			auto const lastOff = lastIdx * dense.mStride;

			std::swap(sparse[pEntity], sparse[last]);
			std::copy_n(dense.mData + lastOff, dense.mStride, dense.mData + offset);

			sparse[last] = INVALID_ENTITY_ID;
			dense.mSize -= 1;

			if (sparse.size() < sparse.capacity() / 2) {
				sparse.resize(sparse.capacity() / 2);
			}

			if (dense.mSize < dense.mCapacity / 2) {
				ResizeDense(dense.mCapacity / 2);
			}

			if (dense.mSize == 0) {
				sparse.clear();
			}
		}

		[[nodiscard]] auto GetValidEntityIds() const noexcept -> std::vector<EntityId> {
			auto const& sparse = mStorage->mSparse;
			auto const& dense  = mStorage->mDense;
			auto        valid  = std::vector<EntityId>{};
			valid.reserve(dense.mSize);

			for (usize index = 0; index < sparse.size(); ++index) {
				if (sparse[index] != INVALID_ENTITY_ID) {
					valid.push_back(static_cast<EntityId>(index));
				}
			}

			return valid;
		}

	private:
		struct Storage final {
			std::vector<EntityId> mSparse = {};
			DenseContainer        mDense  = {};
		};

		std::unique_ptr<Storage> mStorage = {};

		auto ResizeDense(usize const pCapacity) noexcept -> void {
			auto& [_, dense] = *mStorage;

			if (pCapacity <= dense.mCapacity) {
				return;
			}

			auto const newData = new byte[pCapacity * dense.mStride];
			std::copy_n(dense.mData, dense.mSize * dense.mStride, newData);
			delete[] dense.mData;

			dense.mData     = newData;
			dense.mCapacity = pCapacity;
		}
	};
} // kryogenic::ecs

#endif //KRYOGENIC_ECS_COMP_SET_HPP
