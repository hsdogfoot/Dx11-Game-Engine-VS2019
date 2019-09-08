#pragma once
#include "Base.h"

class Resource abstract : public Base
{
public:
	Resource();
	virtual ~Resource() override = default;

	Resource* Clone();

protected:
	virtual void destroy() override = 0;
};