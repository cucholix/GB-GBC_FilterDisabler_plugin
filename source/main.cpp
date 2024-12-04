#include "main.hpp"
#include "NSOGB/main_patch.hpp"

#include "skyline/utils/cpputils.hpp"
#include "skyline/utils/ipc.hpp"

void skyline_main() {
    // populate our own process handle
    Handle h;
    skyline::utils::Ipc::getOwnProcessHandle(&h);
    envSetOwnProcessHandle(h);

    NSOGB_filter_preset_main();
}

extern "C" void skyline_init() {
    skyline::utils::init();
    virtmemSetup();  // needed for libnx JIT

    skyline_main();
}
