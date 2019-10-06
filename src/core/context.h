#pragma once

#include <entt/entt.hpp>
#include <memory>

#include "scomponents/graphics/pipelines.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/camera.h"
#include "scomponents/io/inputs.h"
#include "graphics/render-command.h"

struct Context {
	entt::registry registry;
	std::unique_ptr<RenderCommand> rcommand;
	scomp::Pipelines pipelines;
	scomp::ConstantBuffers cbs;
	scomp::Camera camera;
	scomp::Inputs inputs;
};
