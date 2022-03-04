### 1

Создайте проект Visual Studio (C++ консольное приложение)
В моём случае версия VS 2019 x64

### 2

Скчайте библиотеку SFML https://www.sfml-dev.org/download.php
В моём случае версия 2.5.1

### 3

Cкачайте библиотеку GLEW (Binaries) http://glew.sourceforge.net/index.html
В моём случае версия 2.1.0

### 4

Распакуйте архивы в удобное место
В моём случае C:/libs/{SFML, glew}

### 5

Теперь добавим include файлы

Правая кнопка мыши по солюшину --> Properties --> C/C++ --> General --> Additional Include Derictories
туда добавляем пути: YOUR_LIB_PATH/glew/include, YOUR_LIB_PATH/SFML/include

### 6

Теаерь сами либы

Правая кнопка мыши по солюшину --> Properties --> Linker --> General --> Additional Library Derictories

туда добавляем пути: YOUR_LIB_PATH/glew/libs/release/x64, YOUR_LIB_PATH/SFML/libs

### 7

Добавим зависимости

Правая кнопка мыши по солюшину --> Properties --> Linker --> Input --> Additional Dependencies

добавляем opengl32.lib (системная либа, уже есть в Windows), glew32.lib, sfml-graphics.lib, sfml-system.lib, sfml-window.lib

### 8

Скопировать YOUR_LIB_PATH/glew/bin/x64/glew32.dll --> PROJECT_PATH/PROJECT_NAME
Скопировать YOUR_LIB_PATH/SFML/bin/{sfml-graphics.dll, sfml-system.dll, sfml-window.dll} --> PROJECT_PATH/PROJECT_NAME

PS: Убедитесь что разрядность проекта и библиотек совпадает!!!

PS: что касается UNIX, там все проще, просто нужно передать компилятору ключи (что-то типа -glew -sfml-system ...) и установить sfml (например pacman -S sfml)


Ну и базовый пример кода!

```c++
#include <iostream>

//glew
#include <GL/glew.h>

//sfml
#include <SFML/Window.hpp>

using namespace std;

int main() {
	sf::ContextSettings settings;
	settings.depthBits = 24; // количество битов буффера глубины
	settings.stencilBits = 8; //количество битов буфера трафарета
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	sf::Window window(sf::VideoMode(800, 600, 32), "First Window",
		sf::Style::Titlebar | sf::Style::Close);

	glewExperimental = GL_TRUE; // включить все современные функции ogl

	if (GLEW_OK != glewInit()) { // включить glew
		cout << "Error:: glew not init =(" << endl;
		return -1;
	}

	bool isGo = true;

	while (isGo) {
		sf::Event windowEvent;

		while (window.pollEvent(windowEvent)) { // обработка ивентов
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				isGo = false;
				break;
			default:
				break;
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //отчистка экрана
		glClear(GL_COLOR_BUFFER_BIT); //отчистка экрана

		//...

		window.display();
	}


	window.close();
	return 0;
}

```

