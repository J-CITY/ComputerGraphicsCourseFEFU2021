#pragma once

#include <iostream>

//glew
#include <GL/glew.h>

//sfml
#include <SFML/Window.hpp>

static int e1() {
	sf::ContextSettings settings;
	settings.depthBits = 24; // количество битов буффера глубины
	settings.stencilBits = 8; //количество битов буфера трафарета, используется с буфером глубины для ограничения области рендеринга
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	//sf::VideoMode(ширина, высота, кол-во бидля для цвета на 1 пиксель)
	sf::Window window(sf::VideoMode(800, 600, 32), "First Window",
		sf::Style::Titlebar | sf::Style::Close);

	glewExperimental = GL_TRUE; // включить все современные функции ogl

	if (GLEW_OK != glewInit()) { // включить glew
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

	// Вершинный шейдер
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);        // создаем вертексный шейдер, vertexShader - уникальный id нашего шейдера
	//glShaderSource(шейдер id,
	//				кол-во строк в массиве строк шейдера (у нас все в одну строчку),
	//				сома строка,
	//				длина строки (NULL - значит читает до символа \0))
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // прокидываем программу (текст) в шейдер
	glCompileShader(vertexShader);                              // компилируем шейдер

	// Проверка на наличие ошибок компилирования вершинного шейдера
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // получаем ошибку если есть
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Фрагментный шейдер (аналогично)
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	int shaderProgram = glCreateProgram(); // создаем шейдерную программу и связываем шейдеры с ней
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// проверка на наличие ошибок компилирования связывания шейдеров
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// теперь их можно удалить, мы их связали уже
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices [] = {
		-0.5f, -0.5f, 0.0f, // левая вершина (x y z)
		 0.5f, -0.5f, 0.0f, // правая вершина
		 0.0f,  0.5f, 0.0f  // верхняя вершина   
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO); // сгенерили id для массивов вершин (у нас это один id)
	glBindVertexArray(VAO); // и связали массив с opengl


	glGenBuffers(1, &VBO); // сгенерили буфер данных размером 1
	glBindBuffer(GL_ARRAY_BUFFER, VBO);// и связали буфер с opengl
	//glBufferData(GL_ARRAY_BUFFER - это значит, что этот буфер - атрибуты вершин
	//размер буфера,
	//сам буферб
	//GL_STATIC_DRAW - значит, что данные модифицируются один раз и используются ного раз
	//)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // прокинули наш масив вершин в opengl

	// glVertexAttribPointer(0 - id (потому что это данные для того самого layout (location = 0) in vec3 aPos; из шейдера)
	// размер одной вершины - 3 float,
	// тип элемента,
	// нормализованы ли данные,
	// размер одной вершины, показывает смещение к следующей вершине
	// стартовое смещение в массиве
	// )
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // прописали параметры для объекта
	glEnableVertexAttribArray(0);

	// Обратите внимание, что данное действие разрешено, вызов glVertexAttribPointer()
	// зарегистрировал VBO как привязанный вершинный буферный объект для вершинного атрибута, так что
	// после этого мы можем спокойно выполнить отвязку
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Вы можете отменить привязку VАО после этого, чтобы другие вызовы VАО случайно не изменили этот
	// VAO (но подобное довольно редко случается).
	// Модификация других VAO требует вызов glBindVertexArray(), поэтому мы обычно не снимаем привязку
	// VAO (или VBO), когда это не требуется напрямую
	glBindVertexArray(0);

	// цыкл рендера
	bool isGo = true;
	while (isGo) {

		// обработка ивентов от SFML (тут и мышь и клавиши обрабатываются)
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

		glUseProgram(shaderProgram); // установили нужную шейдерную программу
		glBindVertexArray(VAO);      // установили нужный массив для рендеринга
		glDrawArrays(GL_TRIANGLES, 0, 3); //отрисовали


		window.display();
	}

	// Освобождаем все ресурсы, как только они выполнили свое предназначение
	// не обязательно тк мы завершаем работу программы
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	
	window.close();
	return 0;
}
