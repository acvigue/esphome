#pragma once

#include "esphome/core/component.h"
#include "esphome/components/levoit/levoit.h"
#include "esphome/components/fan/fan.h"

namespace esphome {
namespace levoit {

class LevoitFan : public Component, public fan::Fan {
 public:
  LevoitFan(Levoit *parent) : parent_(parent) {}
  void setup() override;
  void dump_config() override;

  fan::FanTraits get_traits() override;

 protected:
  void control(const fan::FanCall &call) override;
  Levoit *parent_;

 private:
  uint32_t powerMask = 0;
};

}  // namespace levoit
}  // namespace esphome
