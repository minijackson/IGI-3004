#pragma once

#include <sstream>
#include <type_traits>

#include <cstddef>

class buffered_flag {};
class unbuffered_flag {};

/*! \brief Class used to add a buffer among data members to the IFile and OFile classes if the
 *         current flag is `buffered_flag`
 */
template <typename flag>
class DummyFile {};

template <>
class DummyFile<buffered_flag> {
protected:
    std::istringstream buffer;
};
