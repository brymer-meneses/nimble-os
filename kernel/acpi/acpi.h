#pragma once

#include <cstdint>
#include <optional>

namespace ACPI {

  struct SDT {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char OemId[6];
    char OemTableId[8];
    uint32_t OemRevision;
    uint32_t creatorId;
    uint32_t creatorRevision;
  } __attribute__((packed));

  void initialize();

  [[nodiscard]] std::optional<SDT*> findSDT(const char signature[4]);
}
