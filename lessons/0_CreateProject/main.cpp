#include <iostream>

//glew
//#define GLEW_STATIC
#include <GL/glew.h>

//sfml
#include <SFML/Window.hpp>


using namespace std;

int main() {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	sf::Window window(sf::VideoMode(800, 600, 32), "First Window",
		sf::Style::Titlebar | sf::Style::Close);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		cout << "Error:: glew not init =(" << endl;
		return -1;
	}

	bool isGo = true;

	while (isGo) {
		sf::Event windowEvent;

		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				isGo = false;
				break;
			default:
				break;
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//...

		window.display();
	}


	window.close();
	return 0;
}
