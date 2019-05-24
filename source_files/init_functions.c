/******************************************************************************
 * FIRESTARTER - A Processor Stress Test Utility
 * Copyright (C) 2019 TU Dresden, Center for Information Services and High
 * Performance Computing
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact: daniel.hackenberg@tu-dresden.de
 *****************************************************************************/

#include "work.h"

$TEMPLATE sse2_functions_c.init_functions(dest,architectures)

$TEMPLATE avx_functions_c.init_functions(dest,architectures)

$TEMPLATE fma_functions_c.init_functions(dest,architectures)

$TEMPLATE fma4_functions_c.init_functions(dest,architectures)

$TEMPLATE avx512_functions_c.init_functions(dest,architectures)

$TEMPLATE zen_fma_functions_c.init_functions(dest,architectures)

