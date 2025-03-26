#include <chrono>
#include <thread>

#include "rendering/graphics.h"
#include "rendering/shader.h"
#include "rendering/depthShadow.h"
#include "rendering/text.h"

#include "sim/astrobox.h"
#include "sim/earth.h"
#include "sim/clouds.h"


int main()
{
	Camera camera(20.f, 90.f, 0.f, 0.05f);
	Graphics graphics("Earth Render",
						&camera,
						1280, 720,
						4, 6,
						false);

	depthShadow shadowmap;
	Shader shadow_shader("./res/shaders/depthShadow");

	Shader text_shader("./res/shaders/text");
	Text fps_text("", "Prototype.ttf", graphics.getWindow()->getBufferWidth(), graphics.getWindow()->getBufferWidth(), 0.005f, 0.97f, .5f);

	Shader astrobox_shader("./res/shaders/astrobox");
	Astrobox astrobox;

	Shader earth_shader("./res/shaders/earth");
	Earth earth;

	Shader clouds_shader("./res/shaders/clouds");
	Clouds clouds;

	glFinish();

	glfwMakeContextCurrent(NULL);
	std::thread render_thread([&]()
	{
		glfwMakeContextCurrent(graphics.getGLFWwindow());

		GLdouble start_time = 0.0,
				 last_refresh = glfwGetTime();
		int frames = 0;

		while (graphics.windowAlive())
		{
			start_time = glfwGetTime();
			
			shadowmap.bindFBO(&shadow_shader);
			graphics.updateUniforms(&shadow_shader);
			glCullFace(GL_FRONT);
			earth.renderShadow(&shadow_shader);
			glCullFace(GL_BACK);

			glViewport(0, 0, graphics.getWindow()->getBufferWidth(), graphics.getWindow()->getBufferHeight());
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			graphics.clear();

			graphics.updateUniforms(&astrobox_shader);
			graphics.updateUniforms(&earth_shader);
			shadowmap.updateUniforms(&earth_shader);
			graphics.updateUniforms(&clouds_shader);

			astrobox.render(&astrobox_shader);

			// wireframe - good for tessellation debugging
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//glDisable(GL_CULL_FACE);
			earth.render(&earth_shader, shadowmap.getTexID());
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glEnable(GL_CULL_FACE);

			clouds.render(&clouds_shader);


			if (int(glfwGetTime() - last_refresh) > 0)
			{
				fps_text.setText(std::to_string(frames) + "fps");

				frames = 0;
				last_refresh = start_time;
			}
			fps_text.render(text_shader);

			graphics.render();

			frames++;

			// Doesn't work well with vsync
			//std::this_thread::sleep_for(std::chrono::microseconds(16666) - std::chrono::microseconds((long long)((glfwGetTime() - start_time) * 1000000)));
		}
	});

	// Poll input and update simulation
	while (graphics.windowAlive())
	{
		graphics.updateInput();

		// Controls for simulation
		if (graphics.keys()[GLFW_KEY_LEFT])
		{
			//camera.rotate(glm::vec2(0.5f, -0.f));
			earth.rotate(glm::vec3(0.f, -0.5f, 0.f));
			clouds.rotate(-.5f);
		}
		if (graphics.keys()[GLFW_KEY_RIGHT])
		{
			//camera.rotate(glm::vec2(-0.5f, 0.f));
			earth.rotate(glm::vec3(0.f, 0.5f, 0.f));
			clouds.rotate(.5f);
		}

		/*if (graphics.keys()[GLFW_KEY_UP])
			earth.rotate(glm::vec3(-0.5f, 0.f, 0.f));
		if (graphics.keys()[GLFW_KEY_DOWN])
			earth.rotate(glm::vec3(0.5f, 0.f, 0.f));*/

		earth.rotate(glm::vec3(0.f, 0.002f, 0.f));
		clouds.rotate(.003f);

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	render_thread.join();

	return 0;
}
