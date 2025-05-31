#pragma once

namespace ZOQ {
class event_t;
}

namespace ZOQ::itf {

class scheduler_itf {
  public:
    virtual void add(ZOQ::event_t* e) = 0;
    virtual void remove(ZOQ::event_t* e) = 0;
    virtual ~scheduler_itf() = default;
};

}  // namespace ZOQ::itf
