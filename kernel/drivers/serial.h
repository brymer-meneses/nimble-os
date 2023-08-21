#pragma once

namespace serial {
  auto initialize() -> void;

  template<typename ...Args>
  auto println(const char* string, Args... args) -> void {

  }

  template<typename ...Args>
  auto print(const char* string, Args... args) -> void {

  }
}
