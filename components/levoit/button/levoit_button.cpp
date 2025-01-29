#include "esphome/core/log.h"
#include "levoit_button.h"

namespace esphome {
namespace levoit {

static const char *const TAG = "levoit.button";

void LevoitButton::setup() { }

void LevoitButton::press_action() {
  ESP_LOGV(TAG, "Setting button purposecode %u: Pressed", (uint8_t) this->purpose_);
  switch (this->purpose_) {
    case FILTER_RESET:
      this->parent_->set_request_state(static_cast<uint32_t>(LevoitState::FILTER_RESET), 0);
      break;
  }
}

void LevoitButton::dump_config() {
  LOG_BUTTON("", "Levoit Button", this);
  ESP_LOGCONFIG(TAG, "  Button purposecode: %u", (uint8_t) this->purpose_);
}

}  // namespace levoit
}  // namespace esphome
