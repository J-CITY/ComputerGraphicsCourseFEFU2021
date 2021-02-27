#pragma once

#include <iostream>

//glew
#include <GL/glew.h>

//sfml
#include <SFML/Window.hpp>


#include "../3rd/stb/stb_image.h"
#include "../utils/shaderLoader.h"

static int e2() {
	sf::ContextSettings settings;
	settings.depthBits = 24; // количество битов буффера глубины
	settings.stencilBits = 8; //количество битов буфера трафарета, используетс€ с буфером глубины дл€ ограничени€ области рендеринга
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	//sf::VideoMode(ширина, высота, кол-во бидл€ дл€ цвета на 1 пиксель)
	sf::Window window(sf::VideoMode(800, 600, 32), "First Window",
		sf::Style::Titlebar | sf::Style::Close);

	glewExperimental = GL_TRUE; // включить все современные функции ogl

	if (GLEW_OK != glewInit()) { // включить glew
		std::cout << "Error:: glew not init =(" << std::endl;
		return -1;
	}

	auto shaderProgram = LoadShaders("res/shaders/e2.vs", "res/shaders/e2.fs");
	

	float vertices [] = {
		//x      y     z      u     v
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // лева€ нижн€€
		-0.5f, 0.5f, 0.0f,   0.0f, 1.0f,// лева€ верхн€€
		 0.5f, 0.5f, 0.0f,   1.0f, 1.0f,// права€ верхн€€

		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,// лева€ нижн€€
		 0.5f, 0.5f, 0.0f,   1.0f, 1.0f,// права€ верхн€€
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f // права€ нижн€€
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO); // сгенерили id дл€ массивов вершин (у нас это один id)
	glBindVertexArray(VAO); // и св€зали массив с opengl


	glGenBuffers(1, &VBO); // сгенерили буфер данных размером 1
	glBindBuffer(GL_ARRAY_BUFFER, VBO);// и св€зали буфер с opengl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // прокинули наш масив вершин в opengl

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // прописали параметры дл€ объекта
	glEnableVertexAttribArray(0); //тк layout у позиции 0

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// «агрузка и создание текстуры
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // все последующие GL_TEXTURE_2D-операции теперь будут вли€ть на данный текстурный объект

	// ”становка параметров наложени€ текстуры
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // установка метода наложени€ текстуры GL_REPEAT (стандартный метод наложени€)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ”становка параметров фильтрации текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// «агрузка изображени€, создание текстуры и генерирование мипмап-уровней
	int width, height, nrChannels;
	/*
	 * ѕо дефолту текстура перевернута вверх ногами.
	 * Ёто происходит потому, что OpenGL ожидает, что координата 0.0 на оси Y будет
	 * находитьс€ в нижней части изображени€, но изображени€ обычно имеют 0.0 в верхней
	 * части оси Y. stbi_set_flip_vertically_on_load исправит это!
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
	
	// цыкл рендера
	bool isGo = true;
	while (isGo) {

		// обработка ивентов от SFML (тут и мышь и клавиши обрабатываютс€)
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) { // обработка ивентов
			switch (windowEvent.type) {
			case sf::Event::Closed:
				isGo = false;
				break;
			default:
				break;
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //задали цвет отчистки
		glClear(GL_COLOR_BUFFER_BIT);                      //отчистка экрана

		glBindTexture(GL_TEXTURE_2D, texture); //св€зали текстуру
		
		glUseProgram(shaderProgram); // установили нужную шейдерную программу
		glBindVertexArray(VAO);      // установили нужный массив дл€ рендеринга
		glDrawArrays(GL_TRIANGLES, 0, 6); //отрисовали


		window.display();
	}


	window.close();
	return 0;
}
