#pragma once

#include <entt/entt.hpp>
#include <array>
#include <memory>

#include "graphics/render-command.h"

struct Context {
	entt::registry registry;
	std::unique_ptr<RenderCommand> rcommand;
};
