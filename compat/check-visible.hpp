#pragma once

#include "export.hpp"
#include "macros1.h"

class QWidget;

cc_noinline OTR_COMPAT_EXPORT
void set_is_visible(QWidget const& w, bool force = false);

OTR_COMPAT_EXPORT
bool check_is_visible();

OTR_COMPAT_EXPORT
void force_is_visible(bool value);
