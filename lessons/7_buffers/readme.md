# Framebuffer

```c++
unsigned int fbo;
glGenFramebuffers(1, &fbo);

...

glBindFramebuffer(GL_FRAMEBUFFER, fbo);
```

Для того чтобы завершить настройку буфера нужно:

 - прикрепить хотя бы один буфер (буфер цвета, глубины или трафарета);

 - должен быть прикреплен хотя бы один цветовой буфер;

 - формирование всех прикрепляемых объектов также должно быть завершено (зарезервирована память);

 - в каждом буфере должно быть одинаковое количество сэмплов.

 Чтобы проверить, что все корректно

 ```c++
if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
    YOUR_CODE
}
 ```

Открепление у удаление буфера

```c++
glBindFramebuffer(GL_FRAMEBUFFER, 0);
glDeleteFramebuffers(1, &fbo);
```

### Пример

Прикрепим текстуру к фреймбуферу

```c++
glFramebufferTexture2D(
    GL_FRAMEBUFFER, тип фреймбуфера, к которому крепим текстуру
    GL_COLOR_ATTACHMENT0, тип прикрепляемого объекта
    GL_TEXTURE_2D, тип текстуры, которую мы хотим прикрепить
    texture, 
    0 мипмап-уровень
);
```


# Рендер буфер

```c++
unsigned int rbo;
glGenRenderbuffers(1, &rbo);

glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);

glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
```


# Переменные GLSL

`gl_PointSize` float|vert -  задаёт ширину и высоту точки в пикселях

Note: для включения glEnable(GL_PROGRAM_POINT_SIZE);

`gl_VertexID` int|vert - id вершины

`gl_FragCoord` vec3|frag - x y - позиция фрагмента в экранных координатах, z - значение глубины

`gl_FrontFacing` bool|frag - содержит информацию о том, является ли текущий фрагмент частью фронтальной или тыльной грани

`gl_FragDepth` float|frag (0.0-1.0) - глубина фрагмента

При выполнении операции записи в переменную gl_FragDepth вы должны учитывать данный момент. Однако, начиная с OpenGL 4.2, мы можем занять позицию посредника между обеими сторонами, повторно объявляя в начале фрагментного шейдера переменную gl_FragDepth с использованием условия глубины:

```c++
layout (depth_<condition>) out float gl_FragDepth;
```

`any`	Значение по умолчанию. Раннее тестирование глубины отключено.

`greater`	Вы можете только увеличить (в сравнении с gl_FragCoord.z) значение глубины.

`less`	Вы можете только уменьшить (в сравнении с gl_FragCoord.z) значение глубины.

`unchanged`	Если вы будете выполнять запись в gl_FragDepth, то запись будет происходить непосредственно в 
gl_FragCoord.z.


# Интерфейсные блоки

Чтобы помочь нам организовать данные переменные, GLSL предлагает нечто, называемое интерфейсными блоками, позволяющие группировать переменные.
По сути это структуры

```c++
out VS_OUT {
    vec2 TexCoords;
    vec3 light;
} vs_out;
```

Т.е. мы можем передать структурку в другой шейдер.


# uniform-буферы

Позволяет нам объявить набор глобальных uniform-переменных, которые остаются неизменными при использовании любого количества шейдерных программ. 

```c++
layout (std140, binding=2) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
```
### Note
binding=2 - только с версии 4.2

```c++
//c++

unsigned int uboExampleBlock;
glGenBuffers(1, &uboExampleBlock);
glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_STATIC_DRAW); // выделяем 152 байтов памяти
glBindBuffer(GL_UNIFORM_BUFFER, 0);

...

unsigned int index = glGetUniformBlockIndex(shader.ID, "Matrices");   
glUniformBlockBinding(shader.ID, index, 2);
//uniform блок привязали к точке 2


glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboExampleBlock); 
// или
glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboExampleBlock, 0, 152);
//uniform-буфер к той же точке привязали


glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
int b = true; // значения типа bool в GLSL занимают 4 байта, поэтому мы можем сохранить их как int
glBufferSubData(GL_UNIFORM_BUFFER, 144, 4, &b); 
glBindBuffer(GL_UNIFORM_BUFFER, 0);
//прокинули данные в ubo
```