#include "FileContentChangerI.hpp"

FixedQueue<FileContentChangerI::ErrorMain::UniPtr, 10> FileContentChangerI::Errors{};
