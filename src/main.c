#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <glew.h>
#include <GLFW/glfw3.h>

#include "linmath.h"
#include "Shaders.h"

// Global vars
GLFWwindow* window;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "glfw init failed\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(1024, 768, "Hello, World", NULL, NULL);
	if (!window) {
		fprintf(stderr, "glfw: failed to create window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = 1; // Need this for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "glew init failed\n");
		glfwTerminate();
		return -1;
	}

	// Pre loop
	////////
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint vert_arr;
	glGenVertexArrays(1, &vert_arr);
	glBindVertexArray(vert_arr);

	// Make GLSL program from shaders
	GLuint prog = load_shaders("src/VShader.v", "src/FShader.f");

	// An array of 3 vectors which represents 3 vertices
	GLfloat vertices[] = {
		-0.8f, -0.8f, 0.0f,
		 0.8f, -0.8f, 0.0f,
		 0.0f,  0.8f, 0.0f,
	};
	GLuint vert_buff;
	glGenBuffers(1, &vert_buff);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	mat4x4 projection, view, model;
	mat4x4_perspective(
		projection,
		M_PI / 4.0f, // 45 deg fov
		4.0f / 3.0f, // 4:3 ratio
		0.1f,        // display range : 0.1 unit <-> 100 units
		100.0f
	);
	mat4x4_look_at(
		view,
		(vec3) {4.0f, 3.0f, 3.0f}, // eye
		(vec3) {0.0f, 0.0f, 0.0f}, // center
		(vec3) {0.0f, 1.0f, 0.0f}  // up
	);
	mat4x4_identity(model);

	// projection * view * model
	mat4x4 temp, MVP;
	mat4x4_mul(temp, projection, view);
	mat4x4_mul(MVP, temp, model);

	GLuint matrix = glGetUniformLocation(prog, "MVP");
	////////

	while (!glfwWindowShouldClose(window)) {
		// Render
		////////
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(prog); // Use shaders
		glUniformMatrix4fv(matrix, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vert_buff);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*) 0           // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
		////////

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glDeleteVertexArrays(1, &vert_arr);
	glDeleteVertexArrays(1, &vert_buff);
	glDeleteProgram(prog);

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
