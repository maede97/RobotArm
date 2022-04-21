#include <pico/stdlib.h>
#include <robotarm/io.h>

namespace robotarm {

void IO::init_io() {
    stdio_init_all();
}

}  // namespace robotarm