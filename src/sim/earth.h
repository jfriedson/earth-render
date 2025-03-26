#pragma once

#include "../rendering/model.h"


class Earth
{
private:
	Model model;

	Texture diffuseTex,
			specularTex,
			dispTex,
			normTex,
			lightsTex;

	GLfloat initial_rot = 45.f;
	GLfloat axis_tilt = 23.5f;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;


public:
	Earth() :
		model("./res/models/earth/earth.obj"),
		position(glm::vec3(0.f, 0.f, 0.f)),
		origin(glm::vec3(0.f, 0.f, 0.f)),
		rotation(glm::vec3(axis_tilt, 0.f, 0.f)),
		scale(glm::vec3(1.f))
	{
		diffuseTex.LoadTextureLinear("./res/textures/earth/diff_norelief_10k.jpg");
		specularTex.LoadTextureLinear("./res/textures/earth/spec_10k.png");
		dispTex.LoadTextureLinear("./res/textures/earth/disp_10k.jpg");
		normTex.LoadTextureLinear("./res/textures/earth/normal_16k.jpg");
		lightsTex.LoadTextureLinear("./res/textures/earth/lights_10k.jpg");
		
		setModelMatrix();
	}

	~Earth()
	{
	}

	void rotate(glm::vec3 rotate)
	{
		rotation += rotate;

		rotation.x = glm::max(-89.f, glm::min(rotation.x, 89.f));

		if (rotation.y < 0.f)
			rotation.y += 360.f;
		else if (rotation.y > 360.f)
			rotation.y -= 360.f;
	}

	void setModelMatrix()
	{
		glm::mat4 ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, origin);
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::translate(ModelMatrix, position - origin);
		ModelMatrix = glm::scale(ModelMatrix, scale);

		model.setModelMatrix(ModelMatrix);
	}

	void render(Shader *shader, GLuint shadowmap)
	{
		setModelMatrix();

		shader->set1i(0, "diffuseTex");
		shader->set1i(1, "specTex");
		shader->set1i(2, "dispTex");
		shader->set1i(3, "normTex");
		shader->set1i(4, "lightsTex");
		shader->set1i(5, "shadowTex");

		shader->use();

		diffuseTex.UseTexture(0);
		specularTex.UseTexture(1);
		dispTex.UseTexture(2);
		normTex.UseTexture(3);
		lightsTex.UseTexture(4);
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, shadowmap);

		model.render(shader, true);

		shader->unuse();
	}

	void renderShadow(Shader* shader)
	{
		setModelMatrix();

		shader->set1i(0, "dispTex");
		shader->use();

		dispTex.UseTexture(0);

		model.render(shader, true);

		shader->unuse();
	}
};
