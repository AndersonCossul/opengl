
//#define STB_IMAGE_IMPLEMENTATION

#include "gl_utils.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"
#include <iostream>

int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow *g_window = NULL;

int main() {
	restart_gl_log();
	// all the GLFW and GLEW start-up code is moved to here in gl_utils.cpp
	start_gl();
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

								 /* OTHER STUFF GOES HERE NEXT */


								 // set up vertex data (and buffer(s)) and configure vertex attributes
								 // ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors       
		-0.9f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // 
		-0.6f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f, // 
		-0.3f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // 
		0.0f, -0.1f, 0.0f,   1.0f, 1.0f, 0.0f, // 
		0.3f, 0.5f, 0.0f,   1.0f, 0.0f, 1.0f, // 
		0.6f, -0.1f, 0.0f,   0.0f, 1.0f, 1.0f, // 
		0.9f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f // 
	};
	
	unsigned int indices[] = {
		2, 1, 0, // 1 triangle
		3, 1, 2, // 2 triangle
		4, 3, 2, // 3 triangle
		5, 3, 4, // 4 triangle
		6, 5, 4 // 4 triangle
	};

	/* DESSE JEITO GANHA-SE MAIS PERFORMANCE DO QUE VBO, POIS MENOS V�RTICES S�O PASSADOS */

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	char vertex_shader[1024 * 256];
	char fragment_shader[1024 * 256];
	parse_file_into_str("test_vs.glsl", vertex_shader, 1024 * 256);
	parse_file_into_str("test_fs.glsl", fragment_shader, 1024 * 256);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *p = (const GLchar *)vertex_shader;
	glShaderSource(vs, 1, &p, NULL);
	glCompileShader(vs);

	// check for compile errors
	int params = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs);
		print_shader_info_log(vs);
		return 1; // or exit or something
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	p = (const GLchar *)fragment_shader;
	glShaderSource(fs, 1, &p, NULL);
	glCompileShader(fs);

	// check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs);
		print_shader_info_log(fs);
		return 1; // or exit or something
	}

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: could not link shader programme GL index %i\n",
			shader_programme);
		print_programme_info_log(shader_programme);
		return false;
	}

	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK);		// cull back face
	glFrontFace(GL_CW);			// GL_CCW for counter clock-wise


	while (!glfwWindowShouldClose(g_window)) {

		// wipe the drawing surface clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//
		// Note: this call is not necessary, but I like to do it anyway before any
		// time that I call glDrawArrays() so I never use the wrong shader programme
		glUseProgram(shader_programme);

		// Note: this call is not necessary, but I like to do it anyway before any
		// time that I call glDrawArrays() so I never use the wrong vertex data
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);
		// update other events like input handling
		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(g_window, 1);
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
