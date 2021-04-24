# Архитектура

Класс `Core` - содержит все необходимые объекты для работы движка.

```c++
class Core {
    //это не обязательно поля класса, просто здесь все должно быть проинициализированно
    sceneManager;
    window;
    inputManager;
    render;
    timeManager;

    preUpdate();
    update();// здесь апдейтится сцена
    postUpdate();// здесь идет отрисовка

}
```

Класс `Window` - обёртка над sfml Window. окно прокидывает событие нажатие клавиш, положение мыши итд в inputSystem.

Класс `InputManager` - слушает события ои окна и обновляет своё состояние.

```c++
class InputManager {
    bool IsKeyPressed(EKey p_key) const;
	bool IsKeyReleased(EKey p_key) const;
	bool IsMouseButtonPressed(EMouseButton p_button) const;
	bool IsMouseButtonReleased(EMouseButton p_button) const;
	std::pair<int, int> GetMousePosition() const;
	void ClearEvents();
}
```

Класс `Render` - вызывает методы opengl для отрисовки сцены, которую ему дает `SceneManager`.

Класс `Scene` - хранит и оновляет объекты сцены.

```c++
class  Scene: public ObjectIdGenerator<Scene> {
public:
	Scene() = default;
	~Scene();

	void go(); //вызывается при старте сцены
	bool getIsExecute() const;
	
	void Update(float p_deltaTime);//вызывает соответствующий метод у всех объектов

	CreateObject(const std::string& name, const std::string& tag = "");

	bool DestroyActor(p_target);

	FindActorByName(const std::string& name);
	FindActorByTag(const std::string& tag);
	FindActorByID(id);

	std::vector& getObjects();
private:
	bool isExecute = false;
	std::vector<std::shared_ptr<ECS::Object>> objects;
};
```

Класс `Object` - класс игрового объекта.

```c++
class Object {
    public:
		Object(p_actorID, p_name, p_tag, p_enabled);

		~Object();
		const std::string& GetName() const;
		const std::string& GetTag() const;
		void SetName(const std::string& p_name);
		void SetTag(const std::string& p_tag);
		void SetActive(bool p_active);
		bool IsSelfActive() const;
		bool IsActive() const;
		void SetID(p_id);
    	GetID() const;

		void SetParent(Object& p_parent);
		void DetachFromParent();
		bool HasParent() const;
		Object* GetParent() const;
		std::vector<Object*>& GetChildren();

		void OnStart();
		void OnEnable();
		void OnDisable();
		void OnDestroy();
		void OnUpdate(float p_deltaTime);

        AddComponent()
		RemoveComponent()
		template<typename T>
        GetComponent()
    	std::vector<std::shared_ptr<Component>>& GetComponents();
    };
```
