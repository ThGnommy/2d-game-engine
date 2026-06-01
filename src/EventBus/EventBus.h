#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "../Logger/Logger.h"
#include "Event.h"
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <typeindex>
#include <utility>

class IEventCallback {
public:
  virtual ~IEventCallback() = default;

  void Execute(Event &event) { _call(event); }

private:
  virtual void _call(Event &event) = 0;
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
private:
  // Alias for a member function pointer of TOwner that takes a TEvent& and
  // returns void.
  // Modern equivalent: std::function<void(TEvent&)>, but without type erasure
  // overhead.
  using CallbackFunction = void (TOwner::*)(TEvent &);

  TOwner *ownerInstance{};
  CallbackFunction callbackFunction{};

  void _call(Event &event) override {
    std::invoke(callbackFunction, ownerInstance, static_cast<TEvent &>(event));
  }

public:
  EventCallback(TOwner *ownerInstance, CallbackFunction callbackFunction) {
    this->ownerInstance = ownerInstance;
    this->callbackFunction = callbackFunction;
  }

  ~EventCallback() override = default;
};

using HandlerList = std::list<std::unique_ptr<IEventCallback>>;
class EventBus {
public:
  EventBus() { Logger::Log("EventBus constructed called!"); }
  ~EventBus() { Logger::Log("EventBus destructor called!"); }

  /*
   * \brief Subscrive to an event type <T>
   * Example:
   * eventBus->SubscribeToEvent<CollisionEvent>(this,&Game::onCollision);
   * */
  template <typename TOwner, typename TEvent>
  void SubscribeToEvent(TOwner *ownerInstance,
                        void (TOwner::*callbackFunction)(TEvent)) {

    if (!subscribers[typeid(TEvent)].get()) {
      subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
    }

    auto subscriber{std::make_unique<EventCallback<TOwner, TEvent>>(
        ownerInstance, callbackFunction)};
    subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
  }

  template <typename TEvent, typename... TArgs>
  void EmitEvent(TArgs &&...args) {
    if (auto handlers{subscribers[typeid(TEvent)].get()}) {
      for (auto it{handlers->cbegin()}; it != handlers->cend(); it++) {
        auto handler{it->get()};
        TEvent event{std::forward<TArgs>(args)...};
        handler->Execute(event);
      }
    }
  }

private:
  std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers{};
};

#endif
