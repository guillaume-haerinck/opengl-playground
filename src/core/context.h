#pragma once

#include <entt/entt.hpp>
#include <array>

namespace scomp {
	enum SingletonEntities {
		SING_ENTITY_GRAPHIC = 0,
		SING_ENTITY_IO,
		_SINGLETON_ENTITY_MAX
	};
}

struct Context {
	Context() {
		singletonComponents.fill(entt::null);
	}

	entt::registry registry;
	std::array<entt::entity, scomp::SingletonEntities::_SINGLETON_ENTITY_MAX> singletonComponents;
};
