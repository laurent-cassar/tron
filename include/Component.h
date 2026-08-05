#pragma once


class GameObject; 

class Component {
protected:
    GameObject* gameObject; // Réf

public:
    virtual ~Component() = default;

  
    void SetGameObject(GameObject* go) { gameObject = go; }
    GameObject* GetGameObject() const { return gameObject; }

    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {} 
};