#include "pebble_process_info.h"
#include "src/resource_ids.auto.h"

const PebbleProcessInfo __pbl_app_info __attribute__ ((section (".pbl_header"))) = {
  .header = "PBLAPP",
  .struct_version = { PROCESS_INFO_CURRENT_STRUCT_VERSION_MAJOR, PROCESS_INFO_CURRENT_STRUCT_VERSION_MINOR },
  .sdk_version = { PROCESS_INFO_CURRENT_SDK_VERSION_MAJOR, PROCESS_INFO_CURRENT_SDK_VERSION_MINOR },
  .process_version = { 1, 2 },
  .load_size = 0xb6b6,
  .offset = 0xb6b6b6b6,
  .crc = 0xb6b6b6b6,
  .name = "NoLostPhone",
  .company = "krtonga",
  .icon_resource_id = RESOURCE_ID_IC_MENU,
  .sym_table_addr = 0xA7A7A7A7,
  .flags = PROCESS_INFO_WATCH_FACE | PROCESS_INFO_PLATFORM_BASALT,
  .num_reloc_entries = 0xdeadcafe,
  .uuid = { 0x36, 0x3E, 0xFB, 0xA4, 0x9A, 0x0C, 0x47, 0xFD, 0xAE, 0x01, 0xE5, 0x73, 0x78, 0xF3, 0x33, 0x02 },
  .virtual_size = 0xb6b6
};
