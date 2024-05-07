#ifndef KRYOGENIC_CORE_SERVICES_HPP
#define KRYOGENIC_CORE_SERVICES_HPP

#include <stdexcept>
#include <typeindex>
#include <unordered_map>

#include "Kryogenic/Foundation/Types.hpp"

namespace Kryogenic::Core {
	class Services final {
	public:
		class Base {
		public:
			Base() noexcept  = default;
			~Base() noexcept = default;

			Base(Base const& pOther)                        = delete;
			Base(Base&& pOther) noexcept                    = default;
			auto operator=(Base const& pOther) -> Base&     = delete;
			auto operator=(Base&& pOther) noexcept -> Base& = default;
		};

		Services() noexcept  = default;
		~Services() noexcept = default;

		Services(Services const& pOther)                        = delete;
		Services(Services&& pOther) noexcept                    = delete;
		auto operator=(Services const& pOther) -> Services&     = delete;
		auto operator=(Services&& pOther) noexcept -> Services& = delete;

		template<class T>
		static auto Get() -> T& {
			auto const& service = mServices.at(std::type_index(typeid(T)));

			if (nullptr == service) {
				throw std::runtime_error("Service not found.");
			}

			return *static_cast<T*>(service);
		}

		template<class T>
		static auto Set(T* const pService) -> void {
			mServices[std::type_index(typeid(T))] = pService;
		}

		template<class T>
		static auto Remove() -> void {
			mServices.erase(std::type_index(typeid(T)));
		}

	private:
		static inline unordered_map<std::type_index, Base*> mServices = {};
	};
} // kryogenic

#endif //KRYOGENIC_CORE_SERVICES_HPP
