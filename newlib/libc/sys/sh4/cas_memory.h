#pragma once

#include <stddef.h>

#include "impl.h"

CAS_FUN(void *,cas_malloc,(size_t size))
CAS_FUN(void *,cas_free,(void *ptr))
