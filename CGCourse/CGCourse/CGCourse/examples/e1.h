#pragma once

#include <iostream>

//glew
#include <GL/glew.h>

//sfml
#include <SFML/Window.hpp>

static int e1() {
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

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	// ��������� ������
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);        // ������� ���������� ������, vertexShader - ���������� id ������ �������
	//glShaderSource(������ id,
	//				���-�� ����� � ������� ����� ������� (� ��� ��� � ���� �������),
	//				���� ������,
	//				����� ������ (NULL - ������ ������ �� ������� \0))
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // ����������� ��������� (�����) � ������
	glCompileShader(vertexShader);                              // ����������� ������

	// �������� �� ������� ������ �������������� ���������� �������
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // �������� ������ ���� ����
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ����������� ������ (����������)
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	int shaderProgram = glCreateProgram(); // ������� ��������� ��������� � ��������� ������� � ���
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// �������� �� ������� ������ �������������� ���������� ��������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// ������ �� ����� �������, �� �� ������� ���
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices [] = {
		-0.5f, -0.5f, 0.0f, // ����� ������� (x y z)
		 0.5f, -0.5f, 0.0f, // ������ �������
		 0.0f,  0.5f, 0.0f  // ������� �������   
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO); // ��������� id ��� �������� ������ (� ��� ��� ���� id)
	glBindVertexArray(VAO); // � ������� ������ � opengl


	glGenBuffers(1, &VBO); // ��������� ����� ������ �������� 1
	glBindBuffer(GL_ARRAY_BUFFER, VBO);// � ������� ����� � opengl
	//glBufferData(GL_ARRAY_BUFFER - ��� ������, ��� ���� ����� - �������� ������
	//������ ������,
	//��� ������
	//GL_STATIC_DRAW - ������, ��� ������ �������������� ���� ��� � ������������ ���� ���
	//)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ��������� ��� ����� ������ � opengl

	// glVertexAttribPointer(0 - id (������ ��� ��� ������ ��� ���� ������ layout (location = 0) in vec3 aPos; �� �������)
	// ������ ����� ������� - 3 float,
	// ��� ��������,
	// ������������� �� ������,
	// ������ ����� �������, ���������� �������� � ��������� �������
	// ��������� �������� � �������
	// )
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // ��������� ��������� ��� �������
	glEnableVertexAttribArray(0);

	// �������� ��������, ��� ������ �������� ���������, ����� glVertexAttribPointer()
	// ��������������� VBO ��� ����������� ��������� �������� ������ ��� ���������� ��������, ��� ���
	// ����� ����� �� ����� �������� ��������� �������
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// �� ������ �������� �������� V�� ����� �����, ����� ������ ������ V�� �������� �� �������� ����
	// VAO (�� �������� �������� ����� ���������).
	// ����������� ������ VAO ������� ����� glBindVertexArray(), ������� �� ������ �� ������� ��������
	// VAO (��� VBO), ����� ��� �� ��������� ��������
	glBindVertexArray(0);

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

		glUseProgram(shaderProgram); // ���������� ������ ��������� ���������
		glBindVertexArray(VAO);      // ���������� ������ ������ ��� ����������
		glDrawArrays(GL_TRIANGLES, 0, 3); //����������


		window.display();
	}

	// ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������
	// �� ����������� �� �� ��������� ������ ���������
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	
	window.close();
	return 0;
}
