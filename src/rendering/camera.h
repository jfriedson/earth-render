#pragma once

#include <iostream>
#include <vector>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
	Camera(GLfloat radius, GLfloat yaw = 0.f, GLfloat pitch = -90.f, GLfloat mouse_sens = 1.f);
	~Camera();

	void rotate(glm::vec2 delta);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange, GLfloat scroll);

	glm::vec3 getPosition();
	glm::vec3 getDirection();

	glm::mat4 calculateViewMatrix();

private:
	glm::mat4 ViewMatrix;

	glm::vec3 up;

	glm::vec3 target;
	glm::vec3 direction;
	GLfloat radius;

	GLfloat pitch;
	GLfloat yaw;

	glm::vec3 position;

	GLfloat mouse_sens;

	void updateCameraVectors();
};
