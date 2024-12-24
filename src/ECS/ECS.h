#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

constexpr unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

class Entity {
    public:
        Entity(int id) : id(id) {};

        int GetId() const;
    
    private:
        int id{};
};

struct IComponent {
    protected:
        static int nextId;
};

template <typename T>
class Component : public IComponent {
    public:
        static int GetId() {
            static auto id = nextId++;
            return id;
        }
};

class System {
    public:
        System() = default;
        virtual ~System() = default;

        void AddEntity(Entity entity);
        void RemoveEntity(Entity entity);
        std::vector<Entity> GetEntities() const;
        const Signature& GetComponentSignature() const;

        template <typename TComponent> void RequireComponent();

    private:
        Signature componentSignature{};
        std::vector<Entity> entities{};
};

template <typename TComponent> 
void System::RequireComponent() {
    const auto componentId{Component<TComponent>::GetId()};
    componentSignature.set(componentId);
}

class Registry {};

#endif