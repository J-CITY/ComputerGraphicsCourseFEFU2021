
Первое что нужно чделать это создать VAO или Vertex Arrays Object

VAO (Vertex Array Object) -  это такая штука, которая говорит OpenGL, какую часть VBO следует использовать в последующих командах. Чтобы было понятнее, представьте, что VAO представляет собой массив, в элементах которого хранится информация о том, какую часть некого VBO использовать, и как эти данные нужно интерпретировать. Таким образом, один VAO по разным индексам может хранить координаты вершин, их цвета, нормали и прочие данные. Переключившись на нужный VAO мы можем эффективно обращаться к данным, на которые он «указывает», используя только индексы.

Vertex Buffer Object (VBO) — это такое средство OpenGL, позволяющее загружать определенные данные в память GPU. Например, если вы хотите сообщить GPU координаты вершин, цвета или нормали, нужно создать VBO и положить эти данные в него.

```
GLuint VertexArrayID;
glGenVertexArrays(1, &VertexArrayID);
```

1 - размер того, сколько нужно сгенерить имён

Далее подгрузим шейдеры с помошью функции `LoadShaders`, туда передаём путь к вершинному и вертексному шейдеру.

Работу этой функции (`LoadShaders`) разберём отдельно.


```c++
static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 0.0f,  1.0f, 0.0f,
};

GLuint vertexbuffer;
glGenBuffers(1, &vertexbuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),g_vertex_buffer_data, GL_STATIC_DRAW);
```

Далее генерим сам буфер из 3х точек треугольника и связываем его с opengl.

Далее в цикле устанавливаем шейдерную программу и ресуем треугольник

```c++
glUseProgram(programID); //установить шейдерную программу

glBindVertexArray(VertexArrayID); // связываем нужный VAO по id

glEnableVertexAttribArray(0); // выбираем вершины
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // устанавливаем нужный буфер
glVertexAttribPointer(
	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	3,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized or not
	0,                  // stride
	(void*)0            // array buffer offset
);

// Draw the triangle !
glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
glBindVertexArray(0); // сбрасываем VAO
glDisableVertexAttribArray(0); // сбрасываем array
glUseProgram(0); //сбрасываем программу
```
