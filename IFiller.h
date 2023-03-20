#ifndef IREADER_
#define IREADER_
#include "FileTypes.h"

template<typename Fillable>
class IFiller
{
public:
	virtual ~IFiller() = default;
public:
	virtual size_t  read(Fillable&)		  = 0;
	virtual bool is_over() const noexcept = 0;
};

#endif // !IREADER_
