#include "esphome/core/log.h"
#include "levoit_sensor.h"

namespace esphome {
namespace levoit {

static const char *const TAG = "levoit.sensor";

void LevoitSensor::setup() {
  switch (this->purpose_) {
    case LevoitSensorPurpose::AIR_QUALITY:
      this->parent_->register_state_listener(
        static_cast<uint32_t>(LevoitState::AIR_QUALITY_CHANGE),
        [this](uint32_t currentBits) {
          this->publish_state(this->parent_->air_quality);
          this->parent_->set_request_state(0, static_cast<uint32_t>(LevoitState::AIR_QUALITY_CHANGE), false);
        }
      );
      break;
    case LevoitSensorPurpose::PM25:
      this->parent_->register_state_listener(
        static_cast<uint32_t>(LevoitState::PM25_CHANGE) +
        static_cast<uint32_t>(LevoitState::PM25_NAN),
        [this](uint32_t currentBits) {
          if (currentBits & static_cast<uint32_t>(LevoitState::PM25_NAN)) {
            this->publish_state(NAN);
            this->parent_->set_request_state(0, static_cast<uint32_t>(LevoitState::PM25_NAN), false);
          } else {
            uint16_t integer_part = this->parent_->pm25_value / 10;
            uint16_t decimal_part = this->parent_->pm25_value % 10;
            float pm25Publish = integer_part + (decimal_part / 10.0);

            this->publish_state(pm25Publish);
            this->parent_->set_request_state(0, static_cast<uint32_t>(LevoitState::PM25_CHANGE), false);
          }
        }
      );
      break;
  }

}

void LevoitSensor::dump_config() { LOG_SENSOR("", "Levoit Sensor", this); }

}  // namespace levoit
}  // namespace esphome
