#ifndef IREFRESHABLE_FILLER_
#define IREFRESHABLE_FILLER_
#include "IFiller.h"

template<typename Fillable>
class IRefreshableFiller : public IFiller<Fillable>
{
public:
	virtual ~IRefreshableFiller() override = default;
public:
	virtual void refresh() = 0;
};

#endif // !IREFRESHABLE_FILLER_
