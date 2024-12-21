#pragma once
#include <cassert>

#include "ZOQ/scheduler/event.hpp"
#include "ZOQ/scheduler/scheduler_itf.hpp"

namespace ZOQ::scheduler {

class scheduler_t : public scheduler_itf {
  public:
    constexpr scheduler_t() = default;
    // Обрабатываем ивенты за два прохода, чтобы исключить ситуацию, когда внутри ивента заряжеается другой ивент. Тогда
    // его счетчик может быть уменьшен на 1 в этом же цикле
    void tick() {
        // За первый проход уменьшаем счетчики
        // При этом в ивентах, которые должны сработать взводится флаг is_expiring()
        for (event_t* t = _head; t != nullptr; t = t->next())
            t->tick();

        // Во второй проход срабатываем на истекшие ивенты
        for (event_t* t = _head; t != nullptr; t = t->next()) {
            if (t->is_expiring()) {
                t->execute();
                t->reload();
            }
        }
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
