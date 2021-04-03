# Кубические карты

Кубическая карта (англ. «cube map») — это текстура, содержащая 6 независимых 2D-текстур, каждая из которых представляет отдельно взятую сторону текстурированного куба.

Создаем текстуру

```c++
unsigned int textureID;
glGenTextures(1, &textureID);
glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
```

Поскольку кубическая карта содержит 6 текстур: по одной на каждую грань, то мы должны вызвать функцию glTexImage2D() с соответствующими параметрами шесть раз. Однако на этот раз необходимо установить target-параметр текстуры так, чтобы он соответствовал определенной грани кубической карты, сообщая OpenGL для какой стороны кубической карты мы создаем текстуру. Это означает, что мы должны вызвать функцию glTexImage2D() один раз для каждой грани кубической карты.

 - GL_TEXTURE_CUBE_MAP_POSITIVE_X - Справа
 - GL_TEXTURE_CUBE_MAP_NEGATIVE_X - Слева
 - GL_TEXTURE_CUBE_MAP_POSITIVE_Y - Сверху
 - GL_TEXTURE_CUBE_MAP_NEGATIVE_Y - Снизу
 - GL_TEXTURE_CUBE_MAP_POSITIVE_Z - Сзади
 - GL_TEXTURE_CUBE_MAP_NEGATIVE_Z - Спереди

```c++
int width, height, nrChannels;
unsigned char *data;  
for(unsigned int i = 0; i < textures_faces.size(); i++)
{
    data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
    );
}
```

## Fragment shader

```c++
in vec3 textureDir; // вектор направления, представляющий 3D-координату текстуры
uniform samplerCube cubemap; // текстурный сэмплер кубической карты
 
void main()
{             
    FragColor = texture(cubemap, textureDir);
}
```


## Загрузка скайбокса

```c++
vector<std::string> faces = {
    "right.jpg",
    "left.jpg",
    "top.jpg",
    "bottom.jpg",
    "front.jpg",
    "back.jpg"
};

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
 
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
 
    return textureID;
}
```

## Vert shader

```c++
#version 330 core
layout (location = 0) in vec3 aPos;
 
out vec3 TexCoords;
 
uniform mat4 projection;
uniform mat4 view;
 
void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
```

## Frag shader

```c++
#version 330 core
out vec4 FragColor;
 
in vec3 TexCoords;
 
uniform samplerCube skybox;
 
void main()
{    
    FragColor = texture(skybox, TexCoords);
}
```


```c++
glDepthMask(GL_FALSE);
skyboxShader.use();
// ...установка матриц вида и проекции
glBindVertexArray(skyboxVAO);
glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
glDrawArrays(GL_TRIANGLES, 0, 36);
glDepthMask(GL_TRUE);
```