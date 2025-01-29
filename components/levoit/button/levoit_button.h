#pragma once

#include "esphome/core/component.h"
#include "esphome/components/levoit/levoit.h"
#include "esphome/components/button/button.h"

namespace esphome {
namespace levoit {

enum LevoitButtonPurpose : uint8_t { FILTER_RESET };

class LevoitButton : public button::Button, public Component {
 public:
  LevoitButton(Levoit *parent, LevoitButtonPurpose purpose) : parent_(parent), purpose_(purpose) {}
  void setup() override;
  void dump_config() override;

 protected:
  void press_action() override;

  Levoit *parent_;
  LevoitButtonPurpose purpose_;
};

}  // namespace levoit
}  // namespace esphome
