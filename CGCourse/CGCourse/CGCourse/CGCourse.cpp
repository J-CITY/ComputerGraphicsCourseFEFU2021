#include <iostream>

//glew
#include <GL/glew.h>

//sfml
#include <SFML/Window.hpp>


#include "examples/e1.h"
#include "examples/e2.h"
#include "examples/e3.h"

using namespace std;

int main() {
	return e3();
}


/*
#include <iostream>

//glew
//#define GLEW_STATIC
#include <GL/glew.h>

//sfml
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "math.h"
#include "3rd/glm/glm/detail/type_mat.hpp"
#include "3rd/glm/glm/detail/type_vec3.hpp"
#include "3rd/glm/glm/gtc/matrix_transform.inl"
using namespace std;


#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

//imgui
#include "3rd/imgui/imgui.h"
#include "3rd/imgui/imgui-SFML.h"

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


/*
int main() {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "First Window", sf::Style::Titlebar|sf::Style::Close);
	//window.setFramerateLimit(60);
	window.setActive();


	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		cout << "Error:: glew not init =(" << endl;
		return -1;
	}

	ImGui::SFML::Init(window);
	//window.resetGLStates();
	//window.pushGLStates();

	
	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders("shader.vs", "shader.fs");
	
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};
	
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	

	sf::Color bgColor;
	float color[3] = { 0.f, 0.f, 0.f };
	sf::Clock deltaClock;
	//bool show_test_window = true;
	bool show_another_window = true;
	ImVec4 clear_color = ImColor(114, 144, 154);

	bool isGo = true;
	while (isGo) {
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			ImGui::SFML::ProcessEvent(windowEvent);
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				isGo = false;
				break;
			default:
				break;
			}
		}
		


		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Sample window"); // создаём окно

		// Инструмент выбора цвета
		if (ImGui::ColorEdit3("Background color", color)) {
			// код вызывается при изменении значения, поэтому всё
			// обновляется автоматически
			bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
			bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
			bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
		}

		char s[100] = "ImGui + SFML = <3";
		ImGui::InputText("Window title", s, 255);

		if (ImGui::Button("Update window title")) {
			// этот код выполняется, когда юзер жмёт на кнопку
			// здесь можно было бы написать 
			// if(ImGui::InputText(...))
			window.setTitle("!!!!!!");
		}
		ImGui::End(); // end window
		
		//..............................................
		//window.clear();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//ImGui::SFML::Render(window);
		//window.pushGLStates();
		
		// Use our shader
		glUseProgram(programID);


		glBindVertexArray(VertexArrayID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);


	
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
		glBindVertexArray(0);

		glDisableVertexAttribArray(0);
		
		glUseProgram(0);
		

		window.pushGLStates();
		ImGui::SFML::Render(window);
		window.popGLStates();


		window.display();
	}

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	ImGui::SFML::Shutdown();

	window.close();
	return 0;
}
#1#

//using namespace Mathgl;
using namespace glm;

bool isGame = true;

void printM(glm::mat4x4& mat) {
	cout << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << endl;
	cout << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << endl;
	cout << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << endl;
	cout << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << endl;
}

glm::mat4 ViewMatrix = glm::mat4(1.0f);
glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

Mathgl::Mat4 _ViewMatrix = Mathgl::Eye4();
Mathgl::Mat4 _ProjectionMatrix = Mathgl::Eye4();


// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 5);
Mathgl::Vec3 _position(0, 0, 5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;


enum class Key
{
	NONE,
	R,
	L,
	U,
	D
};
Key key;
float posX=0.0f, posY=0.0f;

void computeMatricesFromInputs() {

	float deltaTime = 0.1;


	// Compute new orientation
	horizontalAngle = mouseSpeed * float(800 / 2 - posX);
	verticalAngle = mouseSpeed * float(600 / 2 - posY);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	Mathgl:: Vec3 _direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	Mathgl::Vec3 _right(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);
	Mathgl::Vec3 _up = Mathgl::cross(_right, _direction);

	// Move forward
	if (key == Key::U) {
		position += direction * deltaTime * speed;
		_position += _direction * deltaTime * speed;
		key = Key::NONE;
	}
	// Move backward
	if (key == Key::D) {
		position -= direction * deltaTime * speed;
		_position -= _direction * deltaTime * speed;
		key = Key::NONE;
	}
	// Strafe right
	if (key == Key::R) {
		position += right * deltaTime * speed;
		_position += _right * deltaTime * speed;
		key = Key::NONE;
	}
	// Strafe left
	if (key == Key::L) {
		position -= right * deltaTime * speed;
		_position -= _right * deltaTime * speed;
		key = Key::NONE;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	_ProjectionMatrix = Mathgl::perspective(Mathgl::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	_ViewMatrix = Mathgl::lookAt(
		_position,           // Camera is here
		_position + _direction, // and looks here : at the same position, plus "direction"
		_up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

}

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "First Window", sf::Style::Titlebar | sf::Style::Close);
	//window.setFramerateLimit(60);
	window.setActive();


	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		cout << "Error:: glew not init =(" << endl;
		return -1;
	}

	//ImGui::SFML::Init(window);
	//window.resetGLStates();
	//window.pushGLStates();

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the cameglGenBuffersra than the former one
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	/*
	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	auto Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	auto _Projection = Mathgl::perspective(Mathgl::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	cout << "my  \n" << _Projection << endl;
	printM(Projection);
	
	// Camera matrix
	auto View = glm::lookAt(
		vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		vec3(0, 0, 0), // and looks at the origin
		vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	auto _View = Mathgl::lookAt(
		Mathgl::Vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		Mathgl::Vec3(0, 0, 0), // and looks at the origin
		Mathgl::Vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	cout << "my  \n" << _View << endl;
	printM(View);
	// Model matrix : an identity matrix (model will be at the origin)
	auto Model = mat4(1.0f);
	auto _Model = Mathgl::Eye4();

	cout << "my\n" << _Model << endl;
	printM(Model);
	
	// Our ModelViewProjection : multiplication of our 3 matrices
	//auto MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	auto MVP = _Projection * _View * _Model; // Remember, matrix multiplication is the other way around

	//cout << "my\n" << _MVP << endl;;
	//printM(MVP);
	#1#
	// Load the texture using any two methods
	//GLuint Texture = loadBMP_custom("uvtemplate.bmp");
	//GLuint Texture = loadDDS("uvtemplate.DDS");

	// Get a handle for our "myTextureSampler" uniform
	//GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data [] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	// Two UV coordinatesfor each vertex. They were created with Blender.
	static const GLfloat g_color_buffer_data [] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	do {
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				isGame = false;
				break;
			default:
				break;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				key = Key::L;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				key = Key::R;
			} 
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				key = Key::U;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { 
				key = Key::D;
			}
			if (windowEvent.type == sf::Event::MouseMoved) {
				//posX = sf::Mouse::getPosition().x;
				//posY = sf::Mouse::getPosition().y;

				//sf::Mouse::setPosition(sf::Vector2i(400, 300), window);
			}

		}

		computeMatricesFromInputs();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		Mathgl::Mat4 _ModelMatrix = Mathgl::Eye4();
		auto MVP = _ProjectionMatrix * _ViewMatrix * _ModelMatrix;

		// Clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		//glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		window.display();

	} // Check if the ESC key was pressed or the window was closed
	while (isGame);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	//glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);


	return 0;
}
*/
