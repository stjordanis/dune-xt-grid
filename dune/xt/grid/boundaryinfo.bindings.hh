// This file is part of the dune-xt-grid project:
//   https://github.com/dune-community/dune-xt-grid
// Copyright 2009-2017 dune-xt-grid developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2016 - 2017)

#ifndef DUNE_XT_GRID_BOUNDARYINFO_BINDINGS_HH
#define DUNE_XT_GRID_BOUNDARYINFO_BINDINGS_HH
#if HAVE_DUNE_PYBINDXI

#include <sstream>
#include <type_traits>

#include <dune/pybindxi/pybind11.h>
#include <dune/pybindxi/operators.h>

#include "gridprovider/provider.hh"
#include "boundaryinfo.hh"

namespace Dune {
namespace XT {
namespace Grid {


template <class C>
pybind11::class_<C> bind_BoundaryInfo(pybind11::module& m, const std::string& id /*, const std::string& method_id*/)
{
  namespace py = pybind11;
  using namespace pybind11::literals;

  py::class_<C, BoundaryInfo<typename C::IntersectionType>> c(m, id.c_str(), id.c_str(), py::metaclass());

  c.def(py::init<>());

  c.def_property_readonly_static("static_id", [](const C& /*self*/) { return C::static_id(); });
  c.def("__repr__", [id](const C& /*self*/) { return id + "(" + C::static_id() + ")"; });

  return c;
} // ... bind_BoundaryInfo(...)


} // namespace Grid
} // namespace XT
} // namespace Dune

#endif // HAVE_DUNE_PYBINDXI
#endif // DUNE_XT_GRID_BOUNDARYINFO_BINDINGS_HH
