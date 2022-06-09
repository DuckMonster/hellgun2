#include "my_math.h"
#include <math.h>

bool Math::is_nan(float val)
{
	return isnan(val);
}