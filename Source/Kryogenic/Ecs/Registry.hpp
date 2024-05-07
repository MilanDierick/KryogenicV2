#ifndef KRYOGENIC_ECS_REGISTRY_HPP
#define KRYOGENIC_ECS_REGISTRY_HPP

#include <algorithm>
#include <memory>
#include <typeindex>

#include "Kryogenic/Core/Services.hpp"
#include "Kryogenic/Ecs/ComponentSet.hpp"

namespace Kryogenic::Ecs {
	struct ParentOf final {
	};

	struct ChildOf final {
	};

	class Registry final : public Core::Services::Base {
	public:
		Registry() noexcept  = default;
		~Registry() noexcept = default;

		Registry(Registry const& pOther)                        = delete;
		Registry(Registry&& pOther) noexcept                    = delete;
		auto operator=(Registry const& pOther) -> Registry&     = delete;
		auto operator=(Registry&& pOther) noexcept -> Registry& = delete;

		[[nodiscard]] constexpr auto Create() noexcept -> EntityId {
			if (mFreeEntities.empty()) {
				auto const id = EntityId{static_cast<EntityId>(mEntities.size())};
				mEntities.push_back(id);

				return id;
			}

			auto const id = mFreeEntities.back();
			mFreeEntities.pop_back();

			return id;
		}

		[[nodiscard]] constexpr auto Create(std::string const& pName) noexcept -> EntityId {
			if (mFreeEntities.empty()) {
				auto const id = EntityId{static_cast<EntityId>(mEntities.size())};
				mEntities.push_back(id);
				Set(id, pName);

				return id;
			}

			auto const id = mFreeEntities.back();
			mFreeEntities.pop_back();
			Set(id, pName);

			return id;
		}

		constexpr auto Destroy(EntityId const& pEntity) noexcept -> void {
			std::ranges::for_each(mComponents, [pEntity](auto& pPair) {
				ComponentSet& table = pPair.second;
				table.Erase(pEntity);
			});

			mFreeEntities.push_back(pEntity);
		}

		template<typename T>
		[[nodiscard]] constexpr auto Has(EntityId const& pEntity) const -> b8 {
			auto const& table = GetCompTable<T>();
			return table.contains(pEntity);
		}

		template<typename T>
		[[nodiscard]] constexpr auto Get(EntityId const& pEntity) -> T& {
			auto& table = GetCompTable<T>();
			return *reinterpret_cast<T*>(table.At(pEntity));
		}

		template<typename T>
		[[nodiscard]] constexpr auto Get(EntityId const& pEntity) const -> T const& {
			auto const& table = GetCompTable<T>();
			return *reinterpret_cast<T const*>(table.At(pEntity));
		}

		template<typename T, typename P = std::pair<T, EntityId>>
		[[nodiscard]] constexpr auto Target(EntityId const& pEntity) -> EntityId {
			auto&       table = GetCompTable<P>();
			auto const& pair  = table.template Get<std::pair<T, EntityId>>(pEntity);
			return pair.second;
		}

		[[nodiscard]] constexpr auto GetParent(EntityId const& pChild) -> EntityId {
			return Target<ChildOf>(pChild);
		}

		[[nodiscard]] constexpr auto GetChildren(EntityId const& pParent) -> vector<EntityId> {
			std::vector<EntityId> children = {};

			Query<std::pair<ChildOf, EntityId>>(
				[&children, pParent](auto const& pEntity, auto const& pRelationPair) {
					if (pRelationPair.second == pParent) {
						children.push_back(pEntity);
					}
				});

			return children;
		}

		template<typename T>
		constexpr auto Add(EntityId const& pEntity) -> T& {
			auto& table = GetOrCreateCompTable<T>();
			return table.template insert<T>(pEntity);
		}

		template<class T, typename P = std::pair<T, EntityId>>
		constexpr auto Add(EntityId const& pEntity, EntityId const& pTarget) -> void {
			auto& table = GetOrCreateCompTable<P>();

			if constexpr (std::is_same_v<T, ParentOf>) {
				auto& innerTable = GetOrCreateCompTable<std::pair<ChildOf, EntityId>>();
				(void)innerTable.Insert<std::pair<ChildOf, EntityId>>(
					pTarget, std::make_pair(ChildOf(), pEntity));
			} else if constexpr (std::is_same_v<T, ChildOf>) {
				auto& innerTable = GetOrCreateCompTable<std::pair<ParentOf, EntityId>>();
				(void)innerTable.Insert<std::pair<ParentOf, EntityId>>(
					pTarget, std::make_pair(ParentOf(), pEntity));
			}

			(void)table.template Insert<P>(pEntity, std::make_pair(T(), pTarget));
		}

		template<typename T>
		constexpr auto Set(EntityId const& pEntity, T const& pValue) -> T& {
			auto& table = GetOrCreateCompTable<T>();

			if (table.Contains(pEntity)) {
				return table.Assign(pEntity, pValue);
			}

			return table.Insert(pEntity, pValue);
		}

		template<typename T>
		constexpr auto Remove(EntityId const& pEntity) -> void {
			auto& table = GetOrCreateCompTable<T>();
			table.Erase(pEntity);
		}

		// allows the user to query all entities which have a specific set of components
		// the callback is invoked with the entity id and the component references
		template<typename... COMPS, typename FN>
		constexpr auto Query(FN&& pFunc) -> void {
			auto const tables = std::array{&GetCompTable<COMPS>()...};

			// get the smallest table
			auto const smallest = std::ranges::min_element(tables, [](auto const& pLhs, auto const& pRhs) {
				return pLhs->size() < pRhs->size();
			});

			if (smallest == tables.end()) {
				return;
			}

			auto const& table    = *smallest;
			auto const  entities = table->GetValidEntityIds();

			for (auto const entity: entities) {
				if ((GetCompTable<COMPS>().Contains(entity) && ...)) {
					std::invoke(pFunc, entity, Get<COMPS>(entity)...);
				}
			}
		}

		// allows the user to query all entities which have a specific relation
		// the callback is invoked with the entity that has the relation, and the id of the target entity
		template<typename REL_COMP, typename FN, typename P = std::pair<REL_COMP, EntityId>>
		constexpr auto QueryRelation(FN&& pFunc) -> void {
			auto& table    = GetCompTable<P>();
			auto  entities = table.GetValidEntityIds();

			std::ranges::for_each(entities, [this, &pFunc, &table](auto const& pEntity) {
				auto const& pair = table.template Get<P>(pEntity);
				std::invoke(pFunc, pEntity, pair.second);
			});
		}

	private:
		vector<EntityId> mEntities     = {};
		vector<EntityId> mFreeEntities = {};

		unordered_map<std::type_index, EntityId> mComponentsLookup = {};
		unordered_map<EntityId, ComponentSet>        mComponents       = {};

		ComponentSet mNoOpTable = {};

		template<typename T>
		[[nodiscard]] constexpr auto GetCompTable() -> ComponentSet& {
			auto const type = std::type_index{typeid(T)};

			if (mComponentsLookup.contains(type)) {
				return mComponents.at(mComponentsLookup.at(type));
			}

			return mNoOpTable;
		}

		template<typename T>
		[[nodiscard]] constexpr auto GetOrCreateCompTable() -> ComponentSet& {
			auto const type = std::type_index{typeid(T)};

			if (mComponentsLookup.contains(type)) {
				return mComponents.at(mComponentsLookup.at(type));
			}

			auto const id           = Create();
			mComponentsLookup[type] = id;
			mComponents.emplace(id, ComponentSet(sizeof(T)));

			return mComponents.at(id);
		}
	};
} // kryogenic

#endif //KRYOGENIC_ECS_REGISTRY_HPP
