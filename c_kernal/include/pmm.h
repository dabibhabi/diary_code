#ifndef KERNEL_PMM_H
#define KERNEL_PMM_H

#include "types.h"

// Physical memory manager: tracks free 4 KiB frames via a bitmap.

#define PAGE_SIZE 4096

// TODO: parse multiboot memory map, reserve kernel image + bitmap,
//       mark the rest as free frames
void pmm_init(uint32_t mb_info_addr);

// TODO: find first clear bit in bitmap, set it, return physical address
void* pmm_alloc_frame(void);

// TODO: clear bit for frame at physical address `addr`
void pmm_free_frame(void* addr);

// TODO: debug — total / free / used frame counts
void pmm_stats(size_t* total, size_t* free_frames);

#endif
