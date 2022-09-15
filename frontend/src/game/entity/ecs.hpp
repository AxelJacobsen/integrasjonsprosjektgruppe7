#pragma once

// https://github.com/skypjack/entt
// https://austinmorlan.com/posts/entity_component_system/
// https://mikejsavage.co.uk/blog/cpp-tricks-type-id.html
// http://www.slackerparadise.com/ideas/game-programming/ecs-concepts-part-5-entity/

namespace ecs {
	class Registry;
	using Entity = std::uint32_t;
	static constexpr Entity INVALID_ENTITY = -1;

	static constexpr Entity MAX_ENTITIES = 10000;
	using EntityArray = std::array<Entity, MAX_ENTITIES>;

	using ComponentType = std::uint8_t;
	static constexpr ComponentType MAX_COMPONENTS = 32;
	static constexpr ComponentType INVALID_COMPONENT = -1;

	static constexpr size_t MAX_SYSTEMS = 100;

	using Signature = std::bitset<MAX_COMPONENTS>;

	class EntityManager {
	public:
		EntityManager() {
			for (Entity ent = 0; ent < MAX_ENTITIES; ent++) {
				m_available_entities.push(ent);
			}
		}

		Entity create_entity() {
			if (m_entity_count >= MAX_ENTITIES) {
				logger::warning("Too many entities created");
				return 0;
			}

			const auto id = m_available_entities.front();
			m_available_entities.pop();

			m_entity_count++;

			return id;
		}

		void destroy_entity(Entity entity) {
			if (!valid_entity(entity)) {
				logger::warning("Invalid entity!");
				return;
			}

			// Reset the entity signature
			m_signatures[entity].reset();

			m_available_entities.push(entity);
			m_entity_count--;
		}

		void set_signature(Entity entity, Signature signature) {
			if (!valid_entity(entity)) {
				logger::warning("Invalid entity!");
				return;
			}

			m_signatures[entity] = signature;
		}

		Signature get_signature(Entity entity) {
			if (!valid_entity(entity)) {
				logger::warning("Invalid entity!");
				return m_signatures.at(0);
			}

			return m_signatures.at(entity);
		}

		bool valid_entity(Entity entity) const {
			return entity < MAX_ENTITIES; // no need to check if below zero when unsigned
		}

	private:
		std::queue<Entity> m_available_entities{};
		std::array<Signature, MAX_ENTITIES> m_signatures{};
		std::uint32_t m_entity_count{};
	};

	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;
		virtual void entity_destroyed(Entity entity) = 0;
	};


	template <typename T>
	class ComponentArray : public IComponentArray {
	public:
		static constexpr size_t INVALID = -1;

		ComponentArray() {
			//std::ranges::fill(m_entity_to_index_table, INVALID);
			std::fill(m_entity_to_index_table.begin(), m_entity_to_index_table.end(), INVALID);
		}

		bool has_entity(Entity entity) {
			return m_entity_to_index_table[entity] != INVALID;
		}

		T& insert_data(Entity entity, const T& component) {
			if (has_entity(entity)) {
				logger::warning("You can only add component {} to this entity once\n", typeid(T).name());
				return get_data(entity);
			}

			m_entity_to_index_table[entity] = m_component_count;

			m_index_to_entity_table[m_component_count] = entity;

			T& component_data = m_components[m_component_count];
			component_data = component;

			m_component_count++;

			return component_data;
		}

		/**
		 * \brief Removes component data from entity
		 * \param entity entity to remove data from
		 */
		void remove_data(Entity entity) {
			if (!has_entity(entity)) {
				logger::warning("You cannot remove non existing entity, entity does not have component data!");
				return;
			}

			// Copy element at end into deleted element's place to maintain density
			const size_t removed_entity_component_index = m_entity_to_index_table[entity];
			const size_t last_component = m_component_count - 1;
			const Entity last_component_entity = m_index_to_entity_table[last_component];

			// The index of removed component is overridden with component data of the last/rightmost component
			m_components[removed_entity_component_index] = m_components[last_component];

			// We now update the table so that it points to the new component index (removed one)
			// This is done to pack data together so we don't have internal memory fragmentation for our component buffer

			// Update the last entity to point to the index of the removed one
			m_entity_to_index_table[last_component_entity] = removed_entity_component_index;
			// Update removed entity with the last/rightmost entity
			m_index_to_entity_table[removed_entity_component_index] = last_component_entity;

			// Invalidate the old indices
			m_entity_to_index_table[entity] = INVALID;
			m_index_to_entity_table[last_component] = INVALID_ENTITY;

			m_component_count--;
		}

		T& get_data(Entity entity) {
			VERIFY(has_entity(entity), "Cannot retrieve data for non existing entity, entity does not have component!");

			return m_components[m_entity_to_index_table[entity]];
		}

		void entity_destroyed(Entity entity) override {
			if (has_entity(entity)) {
				// Remove the entity's component if it existed
				remove_data(entity);
			}
		}

	private:
		// Packed array of components
		std::array<T, MAX_ENTITIES> m_components;

		// Translates entity to component index
		std::array<size_t, MAX_ENTITIES> m_entity_to_index_table{};
		// Translates component index to Entity 
		EntityArray m_index_to_entity_table{};

		// Total size of valid entries in the array.
		size_t m_component_count{};
	};

	// TODO: separate type_id for different managers
	inline ComponentType type_count = 0;
	template <typename T>
	inline const ComponentType type_id = type_count++;

	class ComponentManager {
	public:
		template <typename T>
		bool has_component() {
			return m_component_arrays[type_id<T>] != nullptr;
		}

		template <typename T>
		bool entity_has_component(Entity entity) {
			const auto component_array = get_component_array<T>();
			if (!component_array) {
				return false;
			}

			return component_array->has_entity(entity);
		}

		// Registers a new type of component for future use of type T
		template <typename T>
		void register_component() {
			if (has_component<T>()) {
				logger::warning("Cannot register component type more than once!");
				return;
			}

			// TODO: improve this, can check type count when calling get_id, add new ptr or something
			// We just have to hope that get_id isn't called before
			m_component_type_count++;

			m_component_arrays[type_id<T>] = std::make_shared<ComponentArray<T>>();
		}

		// Get the component type of T
		template <typename T>
		ComponentType get_component_type() const {
			return type_id<T>;
		}

		// Adds a component of type T to the entity's component array
		template <typename T>
		T& add_component(Entity entity, const T& component) {
			if (!has_component<T>()) {
				register_component<T>();
			}
			// Add a component to the array for an entity
			return get_component_array<T>()->insert_data(entity, component);
		}

		template <typename T>
		void remove_component(Entity entity) {
			//if (has_component<T>()) {
			// Remove a component from the array for an entity
			get_component_array<T>()->remove_data(entity);
			//}
		}

		template <typename T>
		T& get_component(Entity entity) {
			// Get a reference to a component from the array for an entity
			return get_component_array<T>()->get_data(entity);
		}

		// Removes entity from all component arrays
		void on_entity_destroyed(Entity entity) const {
			int count = 0, index = 0;
			while (count < m_component_type_count) {
				if (m_component_arrays[index]) {
					m_component_arrays[index]->entity_destroyed(entity);
					count++;
				}
				index++;
			}
		}

	private:
		// Holds pointer to all our component arrays
		std::array<std::shared_ptr<IComponentArray>, MAX_COMPONENTS> m_component_arrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType m_component_type_count{};

		template <typename T>
		std::shared_ptr<ComponentArray<T>> get_component_array() {
			return std::static_pointer_cast<ComponentArray<T>>(m_component_arrays[type_id<T>]);
		}
	};

	class System {
	public:
		System() = default;
		System(Registry* registry) : m_registry(registry){}
		std::set<Entity> m_entities{};
		Registry* m_registry = nullptr;
	};

	class SystemManager {
	private:
		template <typename T>
		std::shared_ptr<T> get_system() {
			return std::static_pointer_cast<T>(m_systems[type_id<T>]);
		}

	public:
		template <typename T>
		bool has_system() {
			return m_systems[type_id<T>] != nullptr;
		}

		template <typename T>
		std::shared_ptr<T> register_system(Registry* registry) {
			if (has_system<T>()) {
				logger::warning("System already registered!");
				return get_system<T>();
			}

			auto new_system = std::make_shared<T>(registry);
			// Initialize the system
			m_systems[type_id<T>] = new_system;

			m_available_systems.push_back(type_id<T>);

			return new_system;
		}

		template <typename T>
		void set_signature(Signature signature) {
			if (!has_system<T>()) {
				logger::warning("Cannot use system before it is registered!");
				return;
			}
			// Set the signature for this system
			m_signatures[type_id<T>] = signature;
		}

		void on_entity_destroyed(Entity entity) const {
			// Remove entity from all systems
			for (const auto system_index : m_available_systems) {
				const auto& system = m_systems[system_index];
				system->m_entities.erase(entity);
			}
		}

		void on_entity_signature_changed(Entity entity, Signature entity_signature) const {
			// Notify each system that an entity's signature changed
			for (const auto system_index : m_available_systems) {
				const auto& system = m_systems[system_index];
				const auto& signature = m_signatures[system_index];

				// Entity signature matches system signature - insert into set
				if ((entity_signature & signature) == signature) {
					system->m_entities.insert(entity);
				}
				else {
					// Entity signature does not match system signature - erase from set
					system->m_entities.erase(entity);
				}
			}
		}

	private:
		std::array<Signature, MAX_SYSTEMS> m_signatures{};
		std::array<std::shared_ptr<System>, MAX_SYSTEMS> m_systems{};

		std::deque<size_t> m_available_systems{};
	};

	class Registry {
	public:
		Registry() {
			m_component_manager = std::make_unique<ComponentManager>();
			m_entity_manager = std::make_unique<EntityManager>();
			m_system_manager = std::make_unique<SystemManager>();
		}

		// Implements structured binding iterator
		template <typename ... Components>
		class StructuredView {
		public:
			StructuredView(std::unique_ptr<ComponentManager>& component_manager, EntityArray& entities, Entity entity_count) 
				: m_component_manager(component_manager), m_entities(entities), m_entity_count(entity_count) {
			}

			// Iterator class for structured binding
			class StructuredIterator {
			public:
				StructuredIterator(std::unique_ptr<ComponentManager>& component_manager, EntityArray::iterator iterator)
					: m_component_manager(component_manager), m_iterator(iterator) {
				}

				// Dereference
				std::tuple<Entity, Components&...> operator*() {
					return std::tuple<Entity, Components&...>(*m_iterator, m_component_manager->get_component<Components>(*m_iterator)...);
				}

				// Pre-increment
				StructuredIterator& operator++() {
					++m_iterator;
					return *this;
				}

				// Post-increment
				StructuredIterator operator++(int) {
					StructuredIterator tmp = *this;
					++m_iterator;
					return tmp;
				}

				// Equality
				bool operator==(const StructuredIterator& rhs) {
					return m_iterator == rhs.m_iterator;
				}

				// Inequality
				bool operator!=(const StructuredIterator& rhs) {
					return m_iterator != rhs.m_iterator;
				}

			private:
				std::unique_ptr<ComponentManager>& m_component_manager;
				EntityArray::iterator m_iterator;
			};

			StructuredIterator begin() {
				return StructuredIterator(m_component_manager, m_entities.begin());
			}

			StructuredIterator end() {
				return StructuredIterator(m_component_manager, m_entities.begin() + m_entity_count);
			}

		private:
			std::unique_ptr<ComponentManager>& m_component_manager;
			EntityArray& m_entities;
			Entity m_entity_count{};
		};

		template <typename ... Components>
		class View {
		public:
			View(std::unique_ptr<ComponentManager>& component_manager) : m_component_manager(component_manager){}
			void add(Entity entity) {
				m_entities[m_entity_count++] = entity;
			}
			constexpr EntityArray::iterator begin() {
				return m_entities.begin();
			}

			constexpr EntityArray::iterator end() {
				return m_entities.begin() + m_entity_count;
			}

			constexpr EntityArray::const_iterator begin() const {
				return m_entities.cbegin();
			}

			constexpr EntityArray::const_iterator end() const {
				return m_entities.cbegin() + m_entity_count;
			}

			void each(const auto& callback) {
				for (auto entity : *this) {
					callback(m_component_manager->get_component<Components>(entity)...);
				}
			}

			auto each() {
				return StructuredView<Components...>(m_component_manager, m_entities, m_entity_count);
			}
		private:
			EntityArray m_entities{};
			Entity m_entity_count{};
			std::unique_ptr<ComponentManager>& m_component_manager;
		};

		template <typename ... Components>
		Signature create_signature() {
			Signature signature;
			std::array components{ type_id<Components>... };
			for (auto component : components) {
				signature.set(component);
			}
			return signature;
		}

		// TODO: move to entity manager
		template <typename ... Components>
		auto view() {
			const auto signature = create_signature<Components...>();
			View<Components...> view(m_component_manager);
			for (const auto ent : m_used_entities) {
				const auto sig = m_entity_manager->get_signature(ent);
				if ((sig & signature) == signature) {
					view.add(ent);
				}
			}
			return view;
		}

		// Entity methods
		Entity create_entity() {
			const auto entity = m_entity_manager->create_entity();
			m_used_entities.insert(entity);
			return entity;
		}

		void destroy_entity(Entity entity) {
			m_entity_manager->destroy_entity(entity);

			m_component_manager->on_entity_destroyed(entity);

			m_system_manager->on_entity_destroyed(entity);

			m_used_entities.erase(entity);
		}

		template <typename T>
		T& add_component(Entity entity, const T& component) {
			T& new_component = m_component_manager->add_component<T>(entity, component);

			auto signature = m_entity_manager->get_signature(entity);
			signature.set(m_component_manager->get_component_type<T>(), true);
			m_entity_manager->set_signature(entity, signature);

			m_system_manager->on_entity_signature_changed(entity, signature);

			return new_component;
		}

		template <typename T>
		void remove_component(Entity entity) const {
			m_component_manager->remove_component<T>(entity);

			auto signature = m_entity_manager->get_signature(entity);

			// Set component bit to false
			signature.set(m_component_manager->get_component_type<T>(), false);
			// Update components signature in entity manager
			m_entity_manager->set_signature(entity, signature);

			m_system_manager->on_entity_signature_changed(entity, signature);
		}

		template <typename T>
		T& get_component(Entity entity) {
			return m_component_manager->get_component<T>(entity);
		}

		template <typename T>
		bool has_component() const {
			return m_component_manager->has_component<T>();
		}

		template <typename T>
		bool entity_has_component(Entity entity) const {
			return m_component_manager->entity_has_component<T>(entity);
		}

		template <typename T>
		ComponentType get_component_type() const {
			return m_component_manager->get_component_type<T>();
		}


		// System methods
		template <typename T>
		std::shared_ptr<T> register_system() {
			return m_system_manager->register_system<T>(this);
		}

		template <typename T>
		void set_system_signature(Signature signature) const {
			m_system_manager->set_signature<T>(signature);
		}
 
		template <typename T, typename ... Components>
		void set_system_signature() {
			m_system_manager->set_signature<T>(create_signature<Components...>());
		}

		size_t get_entity_count() const {
			return m_used_entities.size();
		}

	private:
		std::unique_ptr<ComponentManager> m_component_manager{};
		std::unique_ptr<EntityManager> m_entity_manager{};
		std::unique_ptr<SystemManager> m_system_manager{};
		std::set<Entity> m_used_entities{};
	};
}
