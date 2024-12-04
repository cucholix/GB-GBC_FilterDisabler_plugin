#include "skyline/inlinehook/And64InlineHook.hpp"
#include "skyline/utils/cpputils.hpp"
#include "skyline/inlinehook/memcpy_controlled.hpp"

/*08 51 40 F8 56 03 80 52 E1 63 00 91 E0 03 15 AA 
  F6 63 00 39 FF 9B 00 39 E9 93 01 F8 E8 E3 01 F8*/
constinit uint8_t nostalgic[32] = {0x08, 0x51, 0x40, 0xF8, 0x56, 0x03, 0x80, 0x52, 0xE1, 0x63, 0x00, 0x91, 0xE0, 0x03, 0x15, 0xAA, 
                                   0xF6, 0x63, 0x00, 0x39, 0xFF, 0x9B, 0x00, 0x39, 0xE9, 0x93, 0x01, 0xF8, 0xE8, 0xE3, 0x01, 0xF8};
								   
/*08 51 40 F8 56 03 80 52 E1 63 00 91 E0 03 15 AA 
  F6 63 00 39 FF 9B 00 39 E9 93 01 F8 E8 E3 01 F8
  1F 20 03 D5 | (nop sub_sub_710000XXXX)*/  
constinit uint8_t nostalgic_p[36] = {0x08, 0x51, 0x40, 0xF8, 0x56, 0x03, 0x80, 0x52, 0xE1, 0x63, 0x00, 0x91, 0xE0, 0x03, 0x15, 0xAA, 
                                     0xF6, 0x63, 0x00, 0x39, 0xFF, 0x9B, 0x00, 0x39, 0xE9, 0x93, 0x01, 0xF8, 0xE8, 0xE3, 0x01, 0xF8,
								     0x1F, 0x20, 0x03, 0xD5};


bool memcmp_f (const unsigned char *s1, const unsigned char *s2, size_t count) {
    while (count-- > 0)
        if (*s1++ != *s2++) 
            return false;
    return true;
}

size_t getCodeSize() {
        return (size_t)skyline::utils::getRegionAddress(skyline::utils::region::Rodata) - (size_t)skyline::utils::getRegionAddress(skyline::utils::region::Text);
}

/* You must provide full instructions in arrays of uint8_t.
It returns pointer to first found code in main 
or 0 if nothing have been found. */
uintptr_t findTextCode(const uint8_t* code, size_t size) {
    if (size % 4 != 0) return 0;
    uintptr_t addr = (uintptr_t)skyline::utils::getRegionAddress(skyline::utils::region::Text);
    size_t addr_size = getCodeSize();	
    uintptr_t addr_start = addr;
    while (addr != addr_start + addr_size) {
        bool result = memcmp_f((const unsigned char*)addr, code, size);
        if (result) return addr;
        addr += 4;
    }
    return 0;
}

void NSOGB_filter_preset_main() {	
    uintptr_t pointer = findTextCode(&nostalgic[0], sizeof(nostalgic));
    if (pointer) {
        sky_memcpy((void*)pointer, &nostalgic_p[0], sizeof(nostalgic_p));
    }
}
