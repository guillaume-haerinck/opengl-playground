#pragma once

/**
 * @brief Interface used by each exemples. Shows DirectX render capabilities
 */
class IExample {
public:
	virtual ~IExample() {};

	/**
	 * @brief Update the geometry and sends it to the render queue
	 */
	virtual void update() = 0;
	
	/**
	 * @brief Update ImGui Interface
	 */
	virtual void imGuiUpdate() = 0;
};
