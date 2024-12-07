#include "skyline/inlinehook/And64InlineHook.hpp"
#include "skyline/utils/cpputils.hpp"
#include "skyline/inlinehook/memcpy_controlled.hpp"

/*A0 63 00 D1 81 00 80 52 - L_CLASSICS_FILTER_PRESET_CGB_DEFAULT=4*/
constinit uint8_t CGB_DEFAULT[8] = {0xA0, 0x63, 0x00, 0xD1, 0x81, 0x00, 0x80, 0x52};
								   
/*A0 63 00 D1 41 00 80 52 - L_CLASSICS_FILTER_PRESET_CGB_DEFAULT=2*/
constinit uint8_t CGB_DEFAULT_p[8] = {0xA0, 0x63, 0x00, 0xD1, 0x41, 0x00, 0x80, 0x52};

/*B5 00 80 52 A0 63 00 D1 - L_CLASSICS_FILTER_PRESET_CGB_NOSTALGIC=5*/
constinit uint8_t CGB_NOSTALGIC[8] = {0xB5, 0x00, 0x80, 0x52, 0xA0, 0x63, 0x00, 0xD1};
								   
/*75 00 80 52 A0 63 00 D1 - L_CLASSICS_FILTER_PRESET_CGB_NOSTALGIC=3*/
constinit uint8_t CGB_NOSTALGIC_p[8] = {0x75, 0x00, 0x80, 0x52, 0xA0, 0x63, 0x00, 0xD1};

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
    uintptr_t pointer = findTextCode(&CGB_DEFAULT[0], sizeof(CGB_DEFAULT));
    if (pointer) {
        sky_memcpy((void*)pointer, &CGB_DEFAULT_p[0], sizeof(CGB_DEFAULT_p));
    }
	pointer = findTextCode(&CGB_NOSTALGIC[0], sizeof(CGB_NOSTALGIC));
    if (pointer) {
        sky_memcpy((void*)pointer, &CGB_NOSTALGIC_p[0], sizeof(CGB_NOSTALGIC_p));
    }
}
