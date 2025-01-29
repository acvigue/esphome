#include "esphome/core/log.h"
#include "levoit_fan.h"

namespace esphome {
namespace levoit {

static const char *const TAG = "levoit.fan";

void LevoitFan::setup() {
  uint32_t listenMask = this->parent_->fanChangeMask;

  switch (this->parent_->device_model_) {
    // represents power mode
    case LevoitDeviceModel::CORE_400S:
    case LevoitDeviceModel::CORE_200S:
      powerMask |= (static_cast<uint32_t>(LevoitState::POWER) + static_cast<uint32_t>(LevoitState::FAN_MANUAL));
      break;
    default:
      //represensts fan mode
      powerMask |= static_cast<uint32_t>(LevoitState::FAN_MANUAL);
  }

  listenMask |= powerMask;

  this->parent_->register_state_listener(listenMask,
    [this](uint32_t currentBits) {
      bool powerState = (currentBits & powerMask) == powerMask;

      this->state = currentBits & powerState;

      uint8_t newSpeed;

      if (currentBits & static_cast<uint32_t>(LevoitState::FAN_SPEED1))
        newSpeed = 1;
      else if (currentBits & static_cast<uint32_t>(LevoitState::FAN_SPEED2))
        newSpeed = 2;
      else if (currentBits & static_cast<uint32_t>(LevoitState::FAN_SPEED3))
        newSpeed = 3;
      else if (currentBits & static_cast<uint32_t>(LevoitState::FAN_SPEED4))
        newSpeed = 4;

      this->speed = newSpeed;

      this->publish_state();
    }
  );
}


void LevoitFan::dump_config() { LOG_FAN("", "Levoit Fan", this); }

fan::FanTraits LevoitFan::get_traits() {
  switch (this->parent_->device_model_) {
    case LevoitDeviceModel::CORE_400S:
      // 400s has 4 speeds
      return fan::FanTraits(false, true, false, 4);
    default:
      // 200s, 300s has 3 speeds
      return fan::FanTraits(false, true, false, 3);
  }
}

void LevoitFan::control(const fan::FanCall &call) {
  bool newPowerState = this->state;
  uint32_t onMask = 0;
  uint32_t offMask = 0;

  if (call.get_state().has_value()) {
    newPowerState = *call.get_state();
    
    switch (this->parent_->device_model_) {
      case LevoitDeviceModel::CORE_400S:
      case LevoitDeviceModel::CORE_200S:
        if (newPowerState) {
          onMask |= static_cast<uint32_t>(LevoitState::POWER) + static_cast<uint32_t>(LevoitState::FAN_MANUAL);
          offMask &= ~static_cast<uint32_t>(LevoitState::POWER);
        } else {
          onMask &= ~static_cast<uint32_t>(LevoitState::POWER);
          offMask |= static_cast<uint32_t>(LevoitState::POWER);
        }
        break;
      default:
        if (newPowerState) {
          onMask |= static_cast<uint32_t>(LevoitState::FAN_MANUAL);
          offMask &= ~static_cast<uint32_t>(LevoitState::FAN_MANUAL);
        } else {
          onMask &= ~static_cast<uint32_t>(LevoitState::FAN_MANUAL);
          offMask |= static_cast<uint32_t>(LevoitState::FAN_MANUAL);
        }
    }
  }

  if (call.get_speed().has_value()) {
    uint8_t targetSpeed = *call.get_speed();

    switch (targetSpeed) {
      case 0:
        // send power off
        onMask &= ~static_cast<uint32_t>(LevoitState::POWER);
        offMask |= static_cast<uint32_t>(LevoitState::POWER);
        break;
      case 1:
        onMask |= static_cast<uint32_t>(LevoitState::FAN_SPEED1) + static_cast<uint32_t>(LevoitState::FAN_MANUAL);
        offMask &= ~(
          static_cast<uint32_t>(LevoitState::FAN_SPEED2) +
          static_cast<uint32_t>(LevoitState::FAN_SPEED3) +
          static_cast<uint32_t>(LevoitState::FAN_SPEED4)
        );
        break;
      case 2:
        onMask |= static_cast<uint32_t>(LevoitState::FAN_SPEED2) + static_cast<uint32_t>(LevoitState::FAN_MANUAL);
        offMask &= ~(
          static_cast<uint32_t>(LevoitState::FAN_SPEED1) +
          static_cast<uint32_t>(LevoitState::FAN_SPEED3) +
          static_cast<uint32_t>(LevoitState::FAN_SPEED4)
        );
        break;
      case 3:
        onMask |= static_cast<uint32_t>(LevoitState::FAN_SPEED3) + static_cast<uint32_t>(LevoitState::FAN_MANUAL);
        offMask &= ~(
          static_cast<uint32_t>(LevoitState::FAN_SPEED1) +
          static_cast<uint32_t>(LevoitState::FAN_SPEED2) +
          static_cast<uint32_t>(LevoitState::FAN_SPEED4)
        );
        break;
      case 4:
        onMask |= static_cast<uint32_t>(LevoitState::FAN_SPEED4) + static_cast<uint32_t>(LevoitState::FAN_MANUAL);;
        offMask &= ~(
          static_cast<uint32_t>(LevoitState::FAN_SPEED1) +
          static_cast<uint32_t>(LevoitState::FAN_SPEED2) +
          static_cast<uint32_t>(LevoitState::FAN_SPEED3)
        );
        break;
    }
  }

  if (onMask || offMask) {
    // when going from off to fan speed, don't send on
    if (onMask & static_cast<uint32_t>(LevoitState::POWER) && onMask & this->parent_->fanChangeMask)
      onMask &= ~static_cast<uint32_t>(LevoitState::POWER);
    this->parent_->set_request_state(onMask, offMask);
  }
    
}

}  // namespace levoit
}  // namespace esphome
