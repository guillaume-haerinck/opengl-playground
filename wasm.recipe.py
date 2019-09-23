from conans import ConanFile, CMake

class RollGoalWasm(ConanFile):
    settings = {"os": ["Emscripten"]}
    requires = ["entt/3.0.0@skypjack/stable",
      "glm/0.9.8.5@bincrafters/stable", 
      "spdlog/1.3.1@bincrafters/stable",
      "imgui/1.69@bincrafters/stable"]
    generators = ["cmake", "txt"]

    def _configure_cmake(self):
      cmake = CMake(self)
      cmake.configure()
      return cmake

    def build(self):
      cmake = self._configure_cmake()
      cmake.build()
