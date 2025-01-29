#include "esphome/core/log.h"
#include "levoit_switch.h"

namespace esphome {
namespace levoit {

static const char *const TAG = "levoit.switch";

void LevoitSwitch::setup() {
  uint32_t mask = get_mask_(this->purpose_);

  this->parent_->register_state_listener(mask, [this](uint32_t currentBits) {
    uint32_t mask = get_mask_(this->purpose_);
    if (mask > 0)
      this->publish_state((currentBits & mask));
    else
      this->publish_state(NAN);
  });
}

void LevoitSwitch::write_state(bool state) {
  ESP_LOGV(TAG, "Setting switch purposecode %u: %s", (uint8_t) this->purpose_, ONOFF(state));
  uint32_t mask = get_mask_(this->purpose_);
  this->parent_->set_request_state(state ? mask : 0, state ? 0 : mask);
}

void LevoitSwitch::dump_config() {
  LOG_SWITCH("", "Levoit Switch", this);
  ESP_LOGCONFIG(TAG, "  Switch purposecode: %u", (uint8_t) this->purpose_);
}

uint32_t LevoitSwitch::get_mask_(LevoitSwitchPurpose purpose) {
  switch (purpose) {
    case DISPLAY_LOCK: { return(static_cast<uint32_t>(LevoitState::DISPLAY_LOCK)); }
    case MASTER_POWER: { return(static_cast<uint32_t>(LevoitState::POWER)); }
    case DISPLAY_ON: { return(static_cast<uint32_t>(LevoitState::DISPLAY)); }
    default: {
      ESP_LOGE(TAG, "get_mask unknown purpose");
      return(0);
    }
  }
}

}  // namespace levoit
}  // namespace esphome
