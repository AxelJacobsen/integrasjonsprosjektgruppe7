#include <catch2/catch_test_macros.hpp>
#include "../src/game/entity/ecs.hpp"

TEST_CASE( "Entity registration and deletion", "[ecs]" ) {
	ecs::Registry registry;
	REQUIRE(registry.get_entity_count() == 0);
	auto ent = registry.create_entity();
	REQUIRE(registry.get_entity_count() == 1);
    registry.create_entity();
	REQUIRE(registry.get_entity_count() == 2);

	registry.destroy_entity(ent);
	REQUIRE(registry.get_entity_count() == 1);
}


struct Position {
	float x;
	float y;
};

struct Velocity {
	float dx;
	float dy;
};

TEST_CASE("View iteration", "[ecs]") {
	ecs::Registry registry;
	for (int i = 0; i < 10; i++) {
		auto ent = registry.create_entity();
		registry.add_component<Position>(ent, Position{static_cast<float>(i), static_cast<float>(i)});
		registry.add_component<Velocity>(ent, Velocity{static_cast<float>(i), static_cast<float>(i)});
	}

	auto view = registry.view<Position, Velocity>();
	int count = 0;
	view.each([&count](auto _, const Position& pos, const Velocity& vel) {
		VERIFY(pos.x == (float)count && pos.y == (float)count);
		count++;
	});
	VERIFY(count == 10);

	count = 0;
	for (auto [entity, pos, vel] : view.each()) {
		VERIFY(pos.x == (float)count && pos.y == (float)count);
		count++;
	}
	VERIFY(count == 10);

	count = 0;
	for (auto entity : view) {
		const auto pos = registry.get_component<Position>(entity);
		const auto vel = registry.get_component<Velocity>(entity);
		VERIFY(pos.x == (float)count && pos.y == (float)count);
		count++;
	}
	VERIFY(count == 10);
}
