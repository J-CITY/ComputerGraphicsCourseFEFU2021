# Цвета в OpenGLL

Все компоненты RGB(A) от 0.0 до 1.0

Цвета в КГ работают примерно так же как и в реальности. Объекты поклащяют часть цвета источника и "отражают остаток".

```c++
Vec3 lightColor(1.0f, 1.0f, 1.0f);
Vec3 objColor(0.1f, 0.5f, 0.31f);
Vec3 result = lightColor * objColor;
```

## Модель освещения Фонга

Состоит из 3х компонент

`Фоновое/Окружающее освещение (англ. «Ambient lighting»)` — константа окружающего освещения, которая всегда придает объекту некоторый цвет.

```c++
float ambientParam = 0.1;
vec3 ambient = ambientParam * lightColor;

vec3 result = ambient * objectColor;
FragColor = vec4(result, 1.0);
```

`Рассеянное освещение (англ. «Diffuse lighting»)` — имитирует направленное воздействие источника света на объект. Это самый визуально значимый компонент модели освещения. Чем больше объект обращен к источнику света, тем ярче он становится.

Нужно определить, под каким углом луч света падает на фрагмент. Если световой луч перпендикулярен поверхности объекта, то свет оказывает наибольшее воздействие. Для определения угла между лучом света и фрагментом используется нормальный вектор, то есть вектор, перпендикулярный поверхности фрагмента.

Помним, что чем меньше угол между двумя единичными векторами, тем больше скалярное произведение приближено к значению 1. Когда угол между обоими векторами равен 90 градусам, то скалярное произведение равно 0.

```c++
out vec3 FragPos; 
FragPos = vec3(model * vec4(aPos, 1.0)); //все расчеты освещения в мировом пространстве
...

	
in vec3 FragPos;

vec3 norm = normalize(inputNormal);
vec3 lightDir = normalize(lightPos - FragPos);  

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

vec3 result = diffuse * objectColor;
FragColor = vec4(result, 1.0);
```

`Отраженное/Зеркальное освещение (англ. «Specular Lighting»)` — имитирует яркое пятно света, которое появляется на блестящих предметах. Зеркальные блики больше склонны принимать цвет источника света, нежели цвет объекта.

Подобно рассеянному освещению, отраженное освещение основано на векторе направления света и нормальных векторах объекта, но на этот раз оно также зависит и от направления взгляда/

Сначала вычисляется вектор отражения, отражая направление света относительно нормального вектора. Затем мы вычисляем угол между этим вектором отражения и направлением взгляда. Чем меньше угол между ними, тем сильнее воздействие отраженного света. Результирующий эффект заключается в том, что мы видим небольшую подсветку, когда смотрим на направление света, отраженного через поверхность.

```c++
vec3 viewDir = normalize(camPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);  
float specularParam = 0.5;

float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularParam * spec * lightColor;

vec3 result = specular * objectColor;
FragColor = vec4(result, 1.0);
```

reflect() ожидает, что первый вектор будет направлен от источника света к положению фрагмента, но вектор lightDir в настоящее время указывает в обратном направлении: от фрагмента к источнику света.

![Phong](0.png)


```c++
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
```


## Карты освещения

Нужно найти способ, который позволит задать рассеянный цвет для каждого отдельного фрагмента объекта.

Будем использовать текстуру.


```c++
struct Material {
    sampler2D diffuse;
    vec3      specular;
    float     shininess;
};

...

vec3 diffuse = li
ght.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
```

### Карты отраженного цвета

Для этого юзаем ещё одну текстуру.

```c++
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};
```

## Множественные источники света

### Направленный свет

```c++
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, Material material) {
    vec3 lightDir = normalize(-light.direction);
 
    // Рассеянное затенение
    float diff = max(dot(normal, lightDir), 0.0);
 
    // Отраженное затенение
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
 
    // Комбинируем результаты
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}
```

### Точечный свет

```c++
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  
 
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material) {
    vec3 lightDir = normalize(light.position - fragPos);
 
    // Рассеянное затенение
    float diff = max(dot(normal, lightDir), 0.0);
 
    // Отраженное затенение
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
 
    // Затухание
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
 
    // Комбинируем результаты
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 
```

### Прожектор

```c++
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
	
    // Диффузное затенение
    float diff = max(dot(normal, lightDir), 0.0);
	
    // Отраженное затенение
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
    // Затухание
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
	
    // Интенсивность прожектора
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
    // Совмещаем результаты
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
```

### Общий свет

```c++
vec3 result = CalcDirLight(dirLight, norm, viewDir);
 
for(int i = 0; i < N; i++)
  result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

for(int i = 0; i < N; i++)
  result += CalcЫзщеLight(pointLights[i], norm, FragPos, viewDir); 
```

