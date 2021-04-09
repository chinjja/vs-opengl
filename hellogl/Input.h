#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;
class GameEngine;

class Input
{
public:
	Input(GLFWwindow* window = nullptr);
	bool keyPressed(int key) const;
	bool mousePressed(int button) const;
	bool shiftPressed() const;
	bool ctrlPressed() const;
	bool altPressed() const;
	glm::vec2 mousePos() const;
	glm::vec2 mouseDeltaPos() const;

private:
	void updateMousePos();

private:
	GLFWwindow* window_;
	glm::vec2 cur_pos_;
	glm::vec2 prev_pos_;
	glm::vec2 delta_pos_;
	bool shift_;
	bool ctrl_;
	bool alt_;

public:
	friend class GameEngine;
};

