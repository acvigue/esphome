#pragma once

#include "esphome/core/component.h"
#include "esphome/components/levoit/levoit.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace levoit {

enum LevoitSwitchPurpose : uint8_t { DISPLAY_LOCK, MASTER_POWER, DISPLAY_ON };

class LevoitSwitch : public switch_::Switch, public Component {
 public:
  LevoitSwitch(Levoit *parent, LevoitSwitchPurpose purpose) : parent_(parent), purpose_(purpose) {}
  void setup() override;
  void dump_config() override;

 protected:
  void write_state(bool state) override;
  uint32_t get_mask_(LevoitSwitchPurpose purpose);

  Levoit *parent_;
  LevoitSwitchPurpose purpose_;
};

}  // namespace levoit
}  // namespace esphome
