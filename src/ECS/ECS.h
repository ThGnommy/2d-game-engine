#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

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






class IPool {

//  -------->
//  componentsPools
//  -------------------------
//  |   |   |   |   |   |   |   P   | 
//  |---|---|---|---|---|---|   o   |
//  |   |   |   |   |   |   |   o   |
//  |---|---|---|---|---|---|   l   |
//  |   |   |   |   |   |   |   *   V
//  |---|---|---|---|---|---|        
//  |   |   |   |   |   |   |   
//  |---|---|---|---|---|---|   
//  |   |   |   |   |   |   |  > Entity
//  |---|---|---|---|---|---|   
//  |   |   |   |   |   |   |  > Entity
//  -------------------------

public:
    virtual ~IPool() {};
};

// This is a vector of components's entity.
template <typename T>
class Pool : public IPool {
private:
    std::vector<T> data{};

public:
    Pool(int size = 100) {
        data.resize(size);
    }

    virtual ~Pool() = default;

    bool isEmpty() const { 
        return data.empty(); 
    }

    int GetSize() const { 
        return data.size();
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

    const T& Get(const unsigned int index) const {
        return static_cast<T&>(data[index]);
    }

    T& operator [](unsigned int index) {
        return data[index];
    }
};

/*
@brief Entity manager.
*/
class EntityManager {

public:
    EntityManager() = default;

    void Update();
    Entity CreateEntity();

    void AddEntityToSystem(Entity entity);

    void DeleteEntity(Entity* entity);

    template <typename T, typename ...TArgs>
    void AddComponent(const Entity entity, TArgs&& ...args) {
        const auto componentId{Component<T>::GetId()};
        const auto entityId{entity.GetId()};

        // Resize the vector if componentId is greater or equal the componentPools size 
        if (componentId >= componentPools.size()) {
            componentPools.resize(componentId + 1, nullptr);
        }

        if (!componentPools[componentId]) {
            Pool<T>* newComponentPool = new Pool<T>();
            componentPools[componentId] = newComponentPool;
        }

        // Gets the pool of component values for the component type
        Pool<T>* componentPool{Pool<T>(componentPool[componentId])};

        if (entityId >= componentPool->GetSize()) {
            componentPool->resize(numEntities);
        }

        // Create a new component object of type T, 
        // and forward the various paramenters to the constructor
        T newComponent(std::forward<TArgs>(args)...);

        // change the component signature of the entity and set the component id on 
        // the bitset to 1
        entityComponentSignatures[entityId].set(componentId);
    }


private:
    unsigned int numEntities{};

    // Each pool contains all the data for a certain component type.
    // Vector index = component type id
    // Pool index = entity id 
    std::vector<IPool*> componentPools{};

    // Vector of component signatures per entity, saying which component is turned "on" for a given entity
    // Vector index = entity id
    std::vector<Signature> entityComponentSignatures{};

    std::unordered_map<std::type_index, System*> systems{};

    std::set<Entity> entitiesToBeCreated{};
    std::set<Entity> entitiesToBeKilled{};







};

#endif