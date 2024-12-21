#pragma once
#include <cassert>

#include "ZOQ/scheduler/event.hpp"
#include "ZOQ/scheduler/scheduler_itf.hpp"

namespace ZOQ::scheduler {

class scheduler_t : public scheduler_itf {
  public:
    constexpr scheduler_t() = default;
    void tick() {
        for (event_t* t = _head; t != nullptr; t = t->next())
            t->tick();
    }

    scheduler_t(const scheduler_t&) = delete;
    scheduler_t& operator=(const scheduler_t&) = delete;

  private:
    event_t* _head = nullptr;

    void add(event_t* e) override {
        e->set_next(_head);
        _head = e;
    }

    friend class event_t;
};

}  // namespace ZOQ::scheduler
