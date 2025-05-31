#pragma once

#include "o_pin_itf.hpp"
#include "i_pin_itf.hpp"
namespace ZOQ::itf {

class io_pin_itf : public i_pin_itf, public o_pin_itf {};

}  // namespace ZOQ::itf
