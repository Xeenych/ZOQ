#pragma once

#include "ZOQ/itf/timers/it_timer_itf.hpp"
#include "ZOQ/scheduler/event.hpp"
#include "ZOQ/scheduler/scheduler_itf.hpp"

#include <cassert>

namespace ZOQ::scheduler {

using ZOQ::itf::it_timer_itf;

class scheduler_t : public scheduler_itf {
  public:
    constexpr explicit scheduler_t(it_timer_itf& t) : _t{t} {
        _t.set_callback({+[](scheduler_t* arg) { arg->tick(); }, this});
    }
    // Обрабатываем ивенты за три прохода, чтобы исключить ситуацию, когда внутри ивента заряжеается другой ивент. Тогда
    // его счетчик может быть уменьшен на 1 в этом же цикле
    void tick() {
        // За первый проход уменьшаем счетчики
        // При этом в ивентах, которые должны сработать взводится флаг is_expiring()
        for (event_t* t = _head; t != nullptr; t = t->next())
            t->tick();

        // Во второй проход срабатываем на истекшие ивенты
        for (event_t* t = _head; t != nullptr; t = t->next()) {
            if (t->is_expiring())
                t->execute();
        }

        // В третий проход перезагружаем таймеры
        // Если во втормо проходе заново стартует истекший oneshot таймер, то он будет перезагруженнуем и не сработает
        for (event_t* t = _head; t != nullptr; t = t->next()) {
            if (t->is_expiring())
                t->reload();
        }
    }

    scheduler_t(const scheduler_t&) = delete;
    scheduler_t& operator=(const scheduler_t&) = delete;

    ~scheduler_t() override { _t.set_callback({}); }

    void add(event_t* e) override {
        e->set_next(_head);
        _head = e;
    }

    void remove(event_t* e) override {
        // List contains one element
        if (_head == e) {
            _head = _head->next();
            return;
        }

        auto* o = _head;
        auto* prev = o;
        for (; o != nullptr; o = o->next()) {
            if (o == e) {
                prev->set_next(o->next());
                return;
            }
            prev = o;
        }
        assert(false);
    }

  private:
    it_timer_itf& _t;
    event_t* _head = nullptr;
};

}  // namespace ZOQ::scheduler
