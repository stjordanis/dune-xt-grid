// This file is part of the dune-xt-grid project:
//   https://github.com/dune-community/dune-xt-grid
// Copyright 2009-2017 dune-xt-grid developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2016)
//   Rene Milk       (2016)

#include <config.h>

#include <ostream>

#include "interfaces.hh"

namespace Dune {
namespace XT {
namespace Grid {


std::ostream& operator<<(std::ostream& out, const BoundaryType& type)
{
  out << type.id();
  return out;
}


} // namespace Grid
} // namespace XT
} // namespace Dune
