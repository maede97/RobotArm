#pragma once

namespace robotarm {

/**
 * @brief Helper class to initialize the stdio over USB.
 */
class IO {
public:
    static void init_io();
};

}  // namespace robotarm