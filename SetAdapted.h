#ifndef SET_ADAPTED_
#define SET_ADAPTED_
#include "SingularAdapters.h"
#include <set>

ADDAPT_APPANDABLE_CONTAINER(SetAdapted, std::set, insert)

template<typename T>
inline constexpr bool SetAdapted<T>::contains(const T& elem) const noexcept
{
	return container_.contains(elem);
}

#endif // !SET_ADAPTED_
