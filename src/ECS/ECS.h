#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

constexpr unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

class Entity {
    public:

        bool operator==(const Entity& other) const {
            return id == other.GetId();
        }

        bool operator!=(const Entity& other) const {
            return id != other.GetId();
        }

        bool operator>(const Entity& other) const {
            return id > other.GetId();
        }

        bool operator<(const Entity& other) const {
            return id < other.GetId();
        }

        Entity& operator=(const Entity& other) = default;

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

// This is a vector of components's entity.
template <typename T>
class Pool {
private:
    std::vector<T> data{};

public:
    Pool(int size = 100) {
        data.resize(size);
    }

    ~Pool() = default;

    bool isEmpty() const { 
        return data.empty(); 
    }

    int GetSize() const { 
        return data.size(): 
    }

    void Resize(const int n) {
        data.resize(n);        
    }

    void Clear() { 
        data.clear(); 
    }

    void Add(const T object) { 
        data.push_back(object);
    }

    void Set(int index, T object) {
        data[index] = object;
    }

    T& Get(const unsigned int index) {
        return static_cast<T&>(data[index]);
    }

    T& operator [](unsigned int index) {
        return data[index];
    }
};

/*
@brief Entity manager.
*/
class Registry {

private:
    unsigned int numEntities{};

    // Each pool contains all the data for a certain component type.
    std::vector<Pool*> componentsPools{};










};

#endif