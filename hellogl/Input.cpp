#include "Input.h"
#include <GLFW/glfw3.h>

Input::Input(GLFWwindow* window)
	: window_(window),
	cur_pos_(),
	delta_pos_(),
	prev_pos_(),
	shift_(0),
	ctrl_(0),
	alt_(0)
{
}

bool Input::keyPressed(int key) const
{
	if(window_)
		return glfwGetKey(window_, key) == GLFW_PRESS;
	return GLFW_RELEASE;
}

bool Input::mousePressed(int button) const
{
	if (window_)
		return glfwGetMouseButton(window_, button) == GLFW_PRESS;
	return GLFW_RELEASE;
}

bool Input::shiftPressed() const
{
	return shift_;
}

bool Input::ctrlPressed() const
{
	return ctrl_;
}

bool Input::altPressed() const
{
	return alt_;
}

glm::vec2 Input::mousePos() const
{
	return cur_pos_;
}

glm::vec2 Input::mouseDeltaPos() const
{
	return delta_pos_;
}

void Input::updateMousePos()
{
	double x, y;
	glfwGetCursorPos(window_, &x, &y);
	cur_pos_ = glm::vec2(x, y);
	delta_pos_ = cur_pos_ - prev_pos_;
	prev_pos_ = cur_pos_;
}
