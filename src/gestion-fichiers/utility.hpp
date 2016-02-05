#pragma once

#include <sstream>
#include <type_traits>

#include <cstddef>

class buffered_flag {};
class unbuffered_flag {};

/*! \brief Class used to add a buffer among data members to the IFile and OFile classes if the
 *         current flag is `buffered_flag`
 */
template <typename flag, bool isInput>
class DummyFile {};

template <>
class DummyFile<buffered_flag, true> {
protected:
    std::istringstream buffer;
};

template <>
class DummyFile<buffered_flag, false> {
protected:
    std::ostringstream buffer;
};
