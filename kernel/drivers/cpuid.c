#include "cpuid.h"
#include "../libc/string.h"
#define INTEL_MAGIC_NUMBER 0x756e6547
#define AMD_MAGIC_NUMBER 0x68747541
#define UNRECOGNIZED_CPU 0xBADFF

static cpuid_t get_cpuid(icpuid_t cpu);
static icpuid_t detect_cpu(void);
static icpuid_t intel_cpu(void);
static icpuid_t amd_cpu(void);
static icpuid_t generic_cpu(void);

icpuid_t detect_cpu(void) {
    uint32_t ebx, null;
    icpuid_t i_cpu;

    cpuid(0, null, ebx, null, null);

    // Select CPU brand
    switch(ebx) {
        case INTEL_MAGIC_NUMBER:
            i_cpu = intel_cpu();
            break;
        case AMD_MAGIC_NUMBER:
            i_cpu = amd_cpu();
            break;
        default:
            i_cpu = generic_cpu();
            break;
    }

    return i_cpu;
}

icpuid_t intel_cpu(void) {
    uint32_t eax, ebx, null;
    icpuid_t icpu;

    // Fill the structure
    cpuid(1, eax, ebx, null, null);
    icpu.model = (eax >> 4) & 0xF;
    icpu.family = (eax >> 8) & 0xF;
    icpu.type = (eax >> 12) & 0xF;
    icpu.brand = INTEL_MAGIC_NUMBER;
    icpu.stepping = eax & 0xF;
    icpu.reserved = eax >> 14;

    return icpu;
}

icpuid_t amd_cpu(void) {
    uint32_t eax, null;
    icpuid_t icpu;

    // Fill the structure
    cpuid(1, eax, null, null, null);
    icpu.model = (eax >> 4) & 0xF;
    icpu.family = (eax >> 8) & 0xF;
    icpu.stepping = eax & 0xF;
    icpu.reserved = eax >> 12;
    icpu.brand = AMD_MAGIC_NUMBER;

    return icpu;
}

icpuid_t generic_cpu(void) {
    icpuid_t icpu;

    icpu.brand = UNRECOGNIZED_CPU; // Magic number for unknown CPUs

    return icpu;
}

cpuid_t get_cpuid(icpuid_t cpu) {
    cpuid_t cpuid;
    uint8_t model[64];

    // Recognize CPU brand
    if(cpu.brand == AMD_MAGIC_NUMBER) {
        switch(cpu.family) {
            case 4:
                strcpy(model, (uint8_t*)"486 model "); // Set model name
                strcat(model, (void*)cpu.model); // Set model version
                cpuid.model = model;
                break;
            case 5:
                switch(cpu.model) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                        strcpy(model, (uint8_t*)"K6 model "); // Set model name
                        strcat(model, (void*)cpu.model); // Set model version
                        cpuid.model = model;
                        break;
                    case 8:
                        strcpy(model, (uint8_t*)"K6-2 model "); // Set model name
                        strcat(model, (void*)cpu.model); // Set model version
                        cpuid.model = model;
                        break;
                    case 9:
                        strcpy(model, (uint8_t*)"K6-III model "); // Set model name
                        strcat(model, (void*)cpu.model); // Set model version
                        cpuid.model = model;
                        break;
                    default:
                        strcpy(model, (uint8_t*)"K5/K6 model "); // Set model name
                        strcat(model, (void*)cpu.model); // Set model version
                        cpuid.model = model;
                        break;
                }
                break;
            case 6:
                switch(cpu.model) {
                    case 1:
                    case 2:
                    case 3:
                        cpuid.model = (uint8_t*)"Duron model 3";
                        break;
                    case 4:
                        strcpy(model, (uint8_t*)"Athlon model ");
                        strcat(model, (void*)cpu.model);
                        cpuid.model = model;
                        break;
                    case 6:
                        cpuid.model = (uint8_t*)"Athlon MP/Mobile Athlon Model 6";
                        break;
                    case 7:
                        cpuid.model = (uint8_t*)"Mobile Duron Model 7";
                        break;
                    default:
                        strcpy(model, (uint8_t*)"Duron/Athlon model ");
                        strcat(model, (void*)cpu.model);
                        cpuid.model = model;
                        break;
                }
                break;
        }
    } else if(cpu.brand == INTEL_MAGIC_NUMBER) {
        switch(cpu.type) {
            case 0:
                cpuid.type =(uint8_t*)"Original OEM";
                break;
            case 1:
                cpuid.type = (uint8_t*)"Overdrive";
                break;
            case 2:
                cpuid.type = (uint8_t*)"Dual-capable";
                break;
            case 3:
                cpuid.type = (uint8_t*)"Reserved";
                break;
        }

        switch(cpu.family) {
            case 3:
                cpuid.family = (uint8_t*)"i386";
                break;
            case 4:
                cpuid.family = (uint8_t*)"i486";
                break;
            case 5:
                cpuid.family = (uint8_t*)"Pentium II Model 5/Xeon/Celeron";
                break;
            case 6:
                cpuid.family = (uint8_t*)"Pentium Pro";
                break;
            case 15:
                cpuid.family = (uint8_t*)"Pentium 4";
                break;
        }
    } else if(cpu.brand == UNRECOGNIZED_CPU)
        cpuid.family = (uint8_t*)"Generic (x86) CPU";

    return cpuid;
}

uint8_t *get_cpu_type() {
    icpuid_t icpu = detect_cpu(); // Detect CPU brand(Intel, AMD or generic x86)
    cpuid_t cpu_type = get_cpuid(icpu);

    return (uint8_t*)cpu_type.type;
}

uint8_t *get_cpu_family() {
    icpuid_t icpu = detect_cpu(); // Detect CPU brand(Intel, AMD or generic x86)
    cpuid_t cpu_family = get_cpuid(icpu);

    return (uint8_t*)cpu_family.family;

}
