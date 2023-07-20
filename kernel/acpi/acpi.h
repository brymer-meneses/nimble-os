#pragma once

#include <optional>
#include <lib/types.h>
#include <tuple>

namespace ACPI {

  struct AcpiHeader {
    char signature[4];
    u32 length;
    u8 revision;
    u8 checksum;
    char OemId[6];
    char OemTableId[8];
    u32 OemRevision;
    u32 creatorId;
    u32 creatorRevision;
  } __attribute__((packed));

  auto initialize() -> void;

  auto findAcpiHeader(const char signature[4]) -> std::optional<AcpiHeader*>;
  auto isInitialized() -> bool;

}
