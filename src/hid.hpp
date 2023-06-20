#pragma once

namespace components
{

struct InputState;

} // ::components

namespace hid
{


struct ControllerDefinition
{

};

struct DeviceInterface
{
  void check_controls(ControllerDefinition* def, components::InputState* state) const;
};

} // ::hid
