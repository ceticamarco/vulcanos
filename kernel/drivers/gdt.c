#include "gdt.h"

// Internal method
extern void gdt_flush(uint32_t); // Defined on cpu/gdt.asm
static gdt_entry_t construct_null_entry();
static gdt_entry_t construct_entry(gdt_access_t access);
static void init_gdt();

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;


// This method will be called by the kernel
void gdt_setup() {
    init_gdt(); 
}

static void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_entry_t null_segment = construct_null_entry();
    gdt_entry_t kernel_mode_code_segment = construct_entry(
        (struct gdt_access){
            .type = GDT_CODE_TYPE_EXEC_READ,
            .dt   = GDT_CODE_AND_DATA_DESCRIPTOR,
            .dpl  = GDT_RING0,
            .p    = GDT_SEGMENT_PRESENT
        }
    );
    gdt_entry_t kernel_mode_data_segment = construct_entry(
        (struct gdt_access){
            .type = GDT_DATA_TYPE_READ_WRITE,
            .dt   = GDT_CODE_AND_DATA_DESCRIPTOR,
            .dpl  = GDT_RING0,
            .p    = GDT_SEGMENT_PRESENT
        }
    );
    gdt_entry_t user_mode_code_segment = construct_entry(
        (struct gdt_access){
            .type = GDT_CODE_TYPE_EXEC_READ,
            .dt   = GDT_CODE_AND_DATA_DESCRIPTOR,
            .dpl  = GDT_RING3,
            .p    = GDT_SEGMENT_PRESENT
        }
    );
    gdt_entry_t user_mode_data_segment = construct_entry(
        (struct gdt_access){
            .type = GDT_DATA_TYPE_READ_WRITE,
            .dt   = GDT_CODE_AND_DATA_DESCRIPTOR,
            .dpl  = GDT_RING3,
            .p    = GDT_SEGMENT_PRESENT
        }
    );

    gdt_entries[0] = null_segment;
    gdt_entries[1] = kernel_mode_code_segment;
    gdt_entries[2] = kernel_mode_data_segment;
    gdt_entries[3] = user_mode_code_segment;
    gdt_entries[4] = user_mode_data_segment;

    gdt_flush((uint32_t)&gdt_ptr);
}

static gdt_entry_t construct_entry(gdt_access_t access) {
    gdt_entry_t entry = (struct gdt_entry_struct) {
        .base_low = GDT_BASE & 0xFFFF,
        .base_middle = (GDT_BASE >> 16) & 0xFF,
        .base_high = (GDT_BASE >> 24) & 0xFF,
        .limit_low = (GDT_LIMIT & 0xFFFF),
        .access = access,
        .granularity = (struct gdt_granularity) {
            .g = GDT_GRANULARITY_4K,
            .d = GDT_OPERAND_SIZE_32,
            .zero = 0,
            .seglen = GDT_SEGMENT_LENGTH
        }
    };
    return entry;
}

// The only difference is in the access
static gdt_entry_t construct_null_entry() {
    gdt_entry_t null_entry = (struct gdt_entry_struct) {
        .base_low = 0,
        .base_middle = 0,
        .base_high = 0,
        .limit_low = 0,
        .access = (struct gdt_access) {
            .p = 0,
            .dpl = 0,
            .dt = 0,
            .type = 0
        },
        .granularity = (struct gdt_granularity) {
            .g = 0,
            .d = 0,
            .zero = 0,
            .seglen = 0
        }
    };
    return null_entry;
}