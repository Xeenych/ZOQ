#pragma once
#include <cassert>
#include <cstdint>

namespace scheduler {

void init();

class event_t {
  public:
    using fn_t = void (*)(void* arg);
    event_t(const fn_t& f, void* arg) : m_fn(f), m_arg(arg) { timeEvt_head = this; }
    // Запускает отложенное событие
    // ctr - число тиков через которое выполнится отложенное событие
    // interval - интервал для перезагрузки таймера
    // arm(100, 0) - однокртаное выполнение события через 100 тиков
    // arm(100, 500) - выполнить событие черз 100 тиков, а потом выполнять каждые 500 тиков
    void arm(uint32_t ctr, uint32_t interval);
    void disarm(void);
    bool armed() const { return (0 != m_ctr); }

    // call this on SysTick interrupt
    static void tick(void);

  private:
    static inline event_t* timeEvt_head = nullptr;
    const fn_t m_fn = nullptr;
    void* const m_arg = nullptr;

    event_t* const m_next = timeEvt_head;  //! link to next time event in a link-list
    uint32_t m_ctr = 0;                    //! time event down-counter
    uint32_t m_interval = 0;               //! interval for periodic time event

    void execute()
    {
        if (m_fn)
            m_fn(m_arg);
    }
};

}  // namespace scheduler
