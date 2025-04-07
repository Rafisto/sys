#include "mmap.h"
#include "physical.h"

int init_mmap(uint32_t multiboot_info_address)
{
    multiboot2_tag_t *tag;
    for (tag = (multiboot2_tag_t *)(multiboot_info_address + 8);
         tag->type != 0;
         tag = (multiboot2_tag_t *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        if (tag->type == 6)
        {
            multiboot2_mmap_tag_t *mmap_tag = (multiboot2_mmap_tag_t *)tag;
            multiboot2_mmap_entry_t *entry = (multiboot2_mmap_entry_t *)(mmap_tag + 1);
            uint64_t entry_count = (mmap_tag->size - sizeof(multiboot2_mmap_tag_t)) / mmap_tag->entry_size;
            serial_write_format("[mmap][I] found mmap at %x\n", (uint32_t)mmap_tag);

            for (uint64_t i = 0; i < entry_count; i++)
            {
                entry = (multiboot2_mmap_entry_t *)((uint8_t *)entry + mmap_tag->entry_size);
                if (entry->type == 1 && memory_region_count < MAX_USABLE_REGIONS)
                {
                    serial_write_format("[mmap] region %d: base_addr = %x, length = %x, type = %d\n",
                                        i, entry->base_addr, entry->length, entry->type);
                    memory_regions[memory_region_count].base = entry->base_addr;
                    memory_regions[memory_region_count].length = entry->length;
                    memory_region_count++;
                }
            }

            init_physical_memory();
            return 0;
        }
    }
    return -1;
}
