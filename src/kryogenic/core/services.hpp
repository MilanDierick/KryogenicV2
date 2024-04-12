#ifndef KRYOGENIC_CORE_SERVICES_HPP
#define KRYOGENIC_CORE_SERVICES_HPP

#include <stdexcept>
#include <typeindex>
#include <unordered_map>

namespace kryogenic::core {
	class services final {
	public:
		class base {
		public:
			base() noexcept  = default;
			~base() noexcept = default;

			base(base const& p_other)                        = delete;
			base(base&& p_other) noexcept                    = default;
			auto operator=(base const& p_other) -> base&     = delete;
			auto operator=(base&& p_other) noexcept -> base& = default;
		};

		services() noexcept  = default;
		~services() noexcept = default;

		services(services const& p_other)                        = delete;
		services(services&& p_other) noexcept                    = delete;
		auto operator=(services const& p_other) -> services&     = delete;
		auto operator=(services&& p_other) noexcept -> services& = delete;

		template<class T>
		static auto get() -> T& {
			auto const& service = m_services.at(std::type_index(typeid(T)));
			if (nullptr == service) {
				throw std::runtime_error("Service not found.");
			}
			return *static_cast<T*>(service);
		}

		template<class T>
		static auto set(T* const p_service) -> void {
			m_services[std::type_index(typeid(T))] = p_service;
		}

		template<class T>
		static auto remove() -> void {
			m_services.erase(std::type_index(typeid(T)));
		}

	private:
		static inline std::unordered_map<std::type_index, base*> m_services = {
		};
	};
} // kryogenic

#endif //KRYOGENIC_CORE_SERVICES_HPP
