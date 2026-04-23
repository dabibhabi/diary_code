#include "pmm.h"

// TODO: static bitmap + bookkeeping
// static uint32_t* frame_bitmap;
// static size_t    total_frames;
// static size_t    used_frames;

void pmm_init(uint32_t mb_info_addr) {
    // TODO: read mmap from multiboot info, place bitmap after kernel end,
    //       mark reserved regions as used
    (void)mb_info_addr;
}

void* pmm_alloc_frame(void) {
    // TODO: scan bitmap for first 0 bit, set it, return frame address
    return NULL;
}

void pmm_free_frame(void* addr) {
    // TODO: clear the bit matching `addr / PAGE_SIZE`
    (void)addr;
}

void pmm_stats(size_t* total, size_t* free_frames) {
    // TODO: fill out totals
    if (total) *total = 0;
    if (free_frames) *free_frames = 0;
}
