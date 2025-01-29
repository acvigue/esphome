#include "esphome/core/log.h"
#include "levoit_select.h"

namespace esphome {
namespace levoit {

static const char *const TAG = "levoit.select";

void LevoitSelect::setup() {
  switch (this->purpose_) {
    case LevoitSelectPurpose::PURIFIER_NIGHTLIGHT_MODE:
      this->parent_->register_state_listener(
        static_cast<uint32_t>(LevoitState::NIGHTLIGHT_OFF) +
        static_cast<uint32_t>(LevoitState::NIGHTLIGHT_LOW) +
        static_cast<uint32_t>(LevoitState::NIGHTLIGHT_HIGH),
        [this](uint32_t currentBits) {
          if (currentBits & static_cast<uint32_t>(LevoitState::NIGHTLIGHT_HIGH))
            this->publish_state("High");
          else if (currentBits & static_cast<uint32_t>(LevoitState::NIGHTLIGHT_LOW))
            this->publish_state("Low");
          else
            this->publish_state("Off");
        }
      );
      break;
    case LevoitSelectPurpose::PURIFIER_FAN_MODE:
      this->parent_->register_state_listener(
        static_cast<uint32_t>(LevoitState::FAN_MANUAL) +
        static_cast<uint32_t>(LevoitState::FAN_AUTO) +
        static_cast<uint32_t>(LevoitState::FAN_SLEEP),
        [this](uint32_t currentBits) {
          if (currentBits & static_cast<uint32_t>(LevoitState::FAN_MANUAL))
            this->publish_state("Manual");
          else if (currentBits & static_cast<uint32_t>(LevoitState::FAN_AUTO))
            this->publish_state("Auto");
          else if (currentBits & static_cast<uint32_t>(LevoitState::FAN_SLEEP))
            this->publish_state("Sleep");
        }
      );
      break;
    case LevoitSelectPurpose::PURIFIER_AUTO_MODE:
      this->parent_->register_state_listener(
        static_cast<uint32_t>(LevoitState::AUTO_DEFAULT) +
        static_cast<uint32_t>(LevoitState::AUTO_QUIET) +
        static_cast<uint32_t>(LevoitState::AUTO_EFFICIENT),
        [this](uint32_t currentBits) {
          if (currentBits & static_cast<uint32_t>(LevoitState::AUTO_DEFAULT))
            this->publish_state("Default");
          else if (currentBits & static_cast<uint32_t>(LevoitState::AUTO_QUIET))
            this->publish_state("Quiet");
          else if (currentBits & static_cast<uint32_t>(LevoitState::AUTO_EFFICIENT))
            this->publish_state("Effecient");
        }
      );
      break;
  }
}

void LevoitSelect::control(const std::string &value) {
  uint32_t onMask = 0;
  uint32_t offMask = 0;

  switch (this->purpose_) {
    case LevoitSelectPurpose::PURIFIER_NIGHTLIGHT_MODE:
      if (value == "Off") { 
        onMask |= static_cast<uint32_t>(LevoitState::NIGHTLIGHT_OFF);
        offMask |= static_cast<uint32_t>(LevoitState::NIGHTLIGHT_LOW) + static_cast<uint32_t>(LevoitState::NIGHTLIGHT_HIGH);
      } else if (value == "Low") { 
        onMask |= static_cast<uint32_t>(LevoitState::NIGHTLIGHT_LOW);
        offMask |= static_cast<uint32_t>(LevoitState::NIGHTLIGHT_OFF) + static_cast<uint32_t>(LevoitState::NIGHTLIGHT_HIGH);
      } else if (value == "High") { 
        onMask |= static_cast<uint32_t>(LevoitState::NIGHTLIGHT_HIGH);
        offMask |= static_cast<uint32_t>(LevoitState::NIGHTLIGHT_OFF) + static_cast<uint32_t>(LevoitState::NIGHTLIGHT_LOW);
      }
      break;
    case LevoitSelectPurpose::PURIFIER_FAN_MODE:
      if (value == "Manual") {
        onMask |= static_cast<uint32_t>(LevoitState::FAN_MANUAL);
        offMask |= static_cast<uint32_t>(LevoitState::FAN_AUTO) + static_cast<uint32_t>(LevoitState::FAN_SLEEP);
      } else if (value == "Auto") {
        onMask |= static_cast<uint32_t>(LevoitState::FAN_AUTO);
        offMask |= static_cast<uint32_t>(LevoitState::FAN_MANUAL) + static_cast<uint32_t>(LevoitState::FAN_SLEEP);
      } else if (value == "Sleep") {
        onMask |= static_cast<uint32_t>(LevoitState::FAN_SLEEP);
        offMask |= static_cast<uint32_t>(LevoitState::FAN_MANUAL) + static_cast<uint32_t>(LevoitState::FAN_AUTO);
      }
      break;
    case LevoitSelectPurpose::PURIFIER_AUTO_MODE:
      if (value == "Default") {
        onMask |= static_cast<uint32_t>(LevoitState::AUTO_DEFAULT);
        offMask |= static_cast<uint32_t>(LevoitState::AUTO_QUIET) + static_cast<uint32_t>(LevoitState::AUTO_EFFICIENT);
      } else if (value == "Quiet") {
        onMask |= static_cast<uint32_t>(LevoitState::AUTO_QUIET);
        offMask |= static_cast<uint32_t>(LevoitState::AUTO_DEFAULT) + static_cast<uint32_t>(LevoitState::AUTO_EFFICIENT);
      } else if (value == "Effecient") {
        onMask |= static_cast<uint32_t>(LevoitState::AUTO_EFFICIENT);
        offMask |= static_cast<uint32_t>(LevoitState::AUTO_QUIET) + static_cast<uint32_t>(LevoitState::AUTO_DEFAULT);
      }
      break;
  }

  this->parent_->set_request_state(onMask, offMask);
}

void LevoitSelect::dump_config() { LOG_SELECT("", "Levoit Select", this); }

}  // namespace levoit
}  // namespace esphome
