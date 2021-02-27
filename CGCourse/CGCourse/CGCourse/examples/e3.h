#pragma once

#include <iostream>

//glew
#include <GL/glew.h>

//sfml
#include <SFML/Window.hpp>


#include "../3rd/stb/stb_image.h"
#include "../utils/shaderLoader.h"

static int e3() {
	sf::ContextSettings settings;
	settings.depthBits = 24; // ���������� ����� ������� �������
	settings.stencilBits = 8; //���������� ����� ������ ���������, ������������ � ������� ������� ��� ����������� ������� ����������
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	//sf::VideoMode(������, ������, ���-�� ����� ��� ����� �� 1 �������)
	sf::Window window(sf::VideoMode(800, 600, 32), "First Window",
		sf::Style::Titlebar | sf::Style::Close);

	glewExperimental = GL_TRUE; // �������� ��� ����������� ������� ogl

	if (GLEW_OK != glewInit()) { // �������� glew
		std::cout << "Error:: glew not init =(" << std::endl;
		return -1;
	}

	auto shaderProgram = LoadShaders("res/shaders/e2.vs", "res/shaders/e2.fs");
	

	float vertices [] = {
		//x      y     z      u     v
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // ����� ������
		-0.5f, 0.5f, 0.0f,   0.0f, 1.0f,// ����� �������
		 0.5f, 0.5f, 0.0f,   1.0f, 1.0f,// ������ �������
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f // ������ ������
	};

	//EBO �������� ������������ ���� � �� �� ������� � ������ �������������
	unsigned int indices [] = {
		0, 1, 3, // ������ �����������
		1, 2, 3  // ������ �����������
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO); // ��������� id ��� �������� ������ (� ��� ��� ���� id)
	glBindVertexArray(VAO); // � ������� ������ � opengl

	glGenBuffers(1, &VBO); // ��������� ����� ������ �������� 1
	glBindBuffer(GL_ARRAY_BUFFER, VBO);// � ������� ����� � opengl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ��������� ��� ����� ������ � opengl

	//������� � ������� EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // ��������� ��������� ��� �������
	glEnableVertexAttribArray(0); //�� layout � ������� 0

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// �������� � �������� ��������
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // ��� ����������� GL_TEXTURE_2D-�������� ������ ����� ������ �� ������ ���������� ������

	// ��������� ���������� ��������� ��������
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ��������� ������ ��������� �������� GL_REPEAT (����������� ����� ���������)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ��������� ���������� ���������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// �������� �����������, �������� �������� � ������������� ������-�������
	int width, height, nrChannels;
	/*
	 * �� ������� �������� ����������� ����� ������.
	 * ��� ���������� ������, ��� OpenGL �������, ��� ���������� 0.0 �� ��� Y �����
	 * ���������� � ������ ����� �����������, �� ����������� ������ ����� 0.0 � �������
	 * ����� ��� Y. stbi_set_flip_vertically_on_load �������� ���!
	 */
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("res/imgs/1.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	// ���� �������
	bool isGo = true;
	while (isGo) {

		// ��������� ������� �� SFML (��� � ���� � ������� ��������������)
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) { // ��������� �������
			switch (windowEvent.type) {
			case sf::Event::Closed:
				isGo = false;
				break;
			default:
				break;
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //������ ���� ��������
		glClear(GL_COLOR_BUFFER_BIT);                      //�������� ������

		glBindTexture(GL_TEXTURE_2D, texture); //������� ��������
		
		glUseProgram(shaderProgram); // ���������� ������ ��������� ���������
		glBindVertexArray(VAO);      // ���������� ������ ������ ��� ����������
		//glDrawArrays(GL_TRIANGLES, 0, 6); //����������

		//��� ��������� � EBO ����������� glDrawElements
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window.display();
	}


	window.close();
	return 0;
}
