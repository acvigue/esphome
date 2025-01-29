from esphome.components import button
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import ICON_RESTART
from .. import levoit_ns, CONF_LEVOIT_ID, Levoit

DEPENDENCIES = ["levoit"]
CODEOWNERS = ["@acvigue"]

LevoitButton = levoit_ns.class_("LevoitButton", button.Button, cg.Component)
LevoitButtonPurpose = levoit_ns.enum("LevoitButtonPurpose")

CONF_FILTER_RESET = "filter_reset"

CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(CONF_LEVOIT_ID): cv.use_id(Levoit),
        cv.Optional(CONF_FILTER_RESET): button.button_schema(LevoitButton, icon=ICON_RESTART).extend(cv.COMPONENT_SCHEMA),
    })
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_LEVOIT_ID])
    if config_filter_reset := config.get(CONF_FILTER_RESET):
        var = await button.new_button(config_filter_reset, parent, LevoitButtonPurpose.FILTER_RESET)
        await cg.register_component(var, config_filter_reset)