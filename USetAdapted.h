#ifndef USET_ADAPTED_
#define USET_ADAPTED_
#include "SingularAdapters.h"
#include <unordered_set>

ADDAPT_APPANDABLE_CONTAINER(USetAdapted, std::unordered_set, insert)

template<typename T>
inline constexpr bool USetAdapted<T>::contains(const T& elem) const noexcept
{
	return container_.contains(elem);
}

#endif // !USET_ADAPTED_
