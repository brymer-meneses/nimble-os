#pragma once

#define INITIALIZE(module) \
  Kernel::print("Initializing " #module "..."); \
  module::initialize(); \
  Kernel::println("  [Done]");


