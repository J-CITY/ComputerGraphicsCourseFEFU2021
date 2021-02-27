Для такстурирования "треугольников" - используются UV координаты, с помошью них мы можем сопоставить координаты текстуры с координатами треугольников.

## Fragment shader

```c++
#version 330 core
//Входная переменная UV координата
in vec2 UV;

//Выходня переменная полученнйы цвеь в UV позиции
out vec3 color;

//Сама текстура
uniform sampler2D myTextureSampler;

void main(){
    //Берем цвет пикселя в соответствующей UV позиции
    color = texture( myTextureSampler, UV ).rgb;
}
```

## Vertex shader

```c++
#version 330 core

// Входные параметры - вершина треугольника и UV координата
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

// На выход отдаем UV координаты (для фрагментного шейдера)
out vec2 UV;

// MVP матрица - см предыдущий readme
uniform mat4 MVP;

void main(){

    // Выходная позиция вершины в пространстве MVP * position
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

    //Прокидываем UV в fragment shader
    UV = vertexUV;
}
```

В коде c++ нужно создать буфер аналогичный буферу цветного куба. (См предыдущий урок)

```c++
//связали тектсуру с шейдерной программой
GLuint TextureID  = glGetUniformLocation(programID, "myTexture");

static const GLfloat g_uv_buffer_data[] = {
    0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,
    1.000023f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.336024f, 1.0f-0.671877f,
    0.667969f, 1.0f-0.671889f,
    1.000023f, 1.0f-0.000013f,
    0.668104f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.000059f, 1.0f-0.000004f,
    0.335973f, 1.0f-0.335903f,
    0.336098f, 1.0f-0.000071f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.336024f, 1.0f-0.671877f,
    1.000004f, 1.0f-0.671847f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.668104f, 1.0f-0.000013f,
    0.335973f, 1.0f-0.335903f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.668104f, 1.0f-0.000013f,
    0.336098f, 1.0f-0.000071f,
    0.000103f, 1.0f-0.336048f,
    0.000004f, 1.0f-0.671870f,
    0.336024f, 1.0f-0.671877f,
    0.000103f, 1.0f-0.336048f,
    0.336024f, 1.0f-0.671877f,
    0.335973f, 1.0f-0.335903f,
    0.667969f, 1.0f-0.671889f,
    1.000004f, 1.0f-0.671847f,
    0.667979f, 1.0f-0.335851f
};

//выбрали для работы нашу тектуру
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, Texture);
glUniform1i(TextureID, 0);
```

Этот буфер взят с сайта `http://www.opengl-tutorial.org/`. Подобный массив для текстуры можно получить например в Blender.

Вы можете заметить, что каждая вершина учавствуем в нескольких треугольниках, так почему бы просто не вынести это "за скобки".

К счастью в open gl так можно сделать.

## VBO - Vertex Buffer Object

Идея очень проста. Давайте опишим каждую вершину по одному разу. А потом создадим ещё один буфер, в коротом дубем хранить порядок id вершин (порядок того как нужно соеденить эти вершины).

## Квадрат с VBO

```c++

std::vector<unsigned int> vertexbuffer= {
  -0.5, 0, 0.5,
  0.5, 0, 0.5,
  0.5, 0, -0.5,
  -0.5, 0, -0.5,
};
std::vector<unsigned int> indices = {
  0, 1, 2,
  0, 2, 3
};

glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glVertexAttribPointer(
	0,                  // attribute
	3,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized?
	0,                  // stride
	(void*)0            // array buffer offset
);

GLuint elementbuffer;
glGenBuffers(1, &elementbuffer);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices[0], GL_STATIC_DRAW);
...

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

// Draw the triangles !
glDrawElements(
    GL_TRIANGLES,      // mode
    indices.size(),    // count
    GL_UNSIGNED_INT,   // type
    (void*)0           // element array buffer offset
);
```
