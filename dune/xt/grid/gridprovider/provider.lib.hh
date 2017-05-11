// This file is part of the dune-xt-grid project:
//   https://github.com/dune-community/dune-xt-grid
// Copyright 2009-2017 dune-xt-grid developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)

#ifndef DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_HH
#define DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_HH

#include "provider.hh"

#if DUNE_XT_WITH_PYTHON_BINDINGS


#if HAVE_DUNE_FEM
#define _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_DD_SUBDOMAIN(_prefix, _GRID, _type, _backend)                        \
  _prefix typename Dune::XT::Grid::Layer<_GRID,                                                                        \
                                         Dune::XT::Grid::Layers::_type,                                                \
                                         Dune::XT::Grid::Backends::_backend,                                           \
                                         Dune::XT::Grid::DD::SubdomainGrid<_GRID>>::type                               \
  Dune::XT::Grid::GridProvider<_GRID, Dune::XT::Grid::DD::SubdomainGrid<_GRID>>::                                      \
      layer<Dune::XT::Grid::Layers::_type, Dune::XT::Grid::Backends::_backend>(const int) const
#else
#define _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_DD_SUBDOMAIN(_prefix, _GRID, _type, _backend)
#endif

#define _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER(_prefix, _GRID, _type, _backend)                                     \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_DD_SUBDOMAIN(_prefix, _GRID, _type, _backend);                             \
  _prefix                                                                                                              \
      typename Dune::XT::Grid::Layer<_GRID, Dune::XT::Grid::Layers::_type, Dune::XT::Grid::Backends::_backend>::type   \
      Dune::XT::Grid::GridProvider<_GRID>::layer<Dune::XT::Grid::Layers::_type, Dune::XT::Grid::Backends::_backend>(   \
          const int) const

#if HAVE_DUNE_FEM
#define _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_PART_DD_SUBDOMAIN(_prefix, _GRID, _type)                             \
  _prefix typename Dune::XT::Grid::Layer<_GRID,                                                                        \
                                         Dune::XT::Grid::Layers::_type,                                                \
                                         Dune::XT::Grid::Backends::part,                                               \
                                         Dune::XT::Grid::DD::SubdomainGrid<_GRID>>::type                               \
  Dune::XT::Grid::GridProvider<_GRID, Dune::XT::Grid::DD::SubdomainGrid<_GRID>>::layer<Dune::XT::Grid::Layers::_type>( \
      const int)

#define _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_PART(_prefix, _GRID, _type)                                          \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_PART_DD_SUBDOMAIN(_prefix, _GRID, _type);                                  \
  _prefix typename Dune::XT::Grid::Layer<_GRID, Dune::XT::Grid::Layers::_type, Dune::XT::Grid::Backends::part>::type   \
  Dune::XT::Grid::GridProvider<_GRID>::layer<Dune::XT::Grid::Layers::_type>(const int)

#define _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_PART(_prefix, _GRID)                                                       \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER(_prefix, _GRID, level, part);                                              \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER(_prefix, _GRID, leaf, part);                                               \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER(_prefix, _GRID, adaptive_leaf, part);                                      \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_DD_SUBDOMAIN(_prefix, _GRID, dd_subdomain, part);                          \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_PART(_prefix, _GRID, level);                                               \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_PART(_prefix, _GRID, leaf);                                                \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_PART(_prefix, _GRID, adaptive_leaf);                                       \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER_PART_DD_SUBDOMAIN(_prefix, _GRID, dd_subdomain)
#else
#define _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_PART(_prefix, _GRID)
#endif

#define _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_ALL_LAYERS(_prefix, _GRID)                                                 \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_PART(_prefix, _GRID);                                                            \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER(_prefix, _GRID, level, view);                                              \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_LAYER(_prefix, _GRID, leaf, view)

#if HAVE_DUNE_FEM
#define _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_DD_SUBDOMAIN(_prefix, _GRID)                                               \
  _prefix class Dune::XT::Grid::GridProvider<_GRID, Dune::XT::Grid::DD::SubdomainGrid<_GRID>>
#else
#define _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_DD_SUBDOMAIN(_prefix, _GRID)
#endif

#define DUNE_XT_GRID_PROVIDER_PROVIDER_LIB(_prefix, _GRID)                                                             \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_DD_SUBDOMAIN(_prefix, _GRID);                                                    \
  _prefix class Dune::XT::Grid::GridProvider<_GRID>;                                                                   \
  _DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_ALL_LAYERS(_prefix, _GRID)

#if HAVE_DUNE_ALUGRID
DUNE_XT_GRID_PROVIDER_PROVIDER_LIB(extern template, ALU_2D_SIMPLEX_CONFORMING);
#endif
DUNE_XT_GRID_PROVIDER_PROVIDER_LIB(extern template, YASP_1D_EQUIDISTANT_OFFSET);
DUNE_XT_GRID_PROVIDER_PROVIDER_LIB(extern template, YASP_2D_EQUIDISTANT_OFFSET);
DUNE_XT_GRID_PROVIDER_PROVIDER_LIB(extern template, YASP_3D_EQUIDISTANT_OFFSET);


#endif // DUNE_XT_WITH_PYTHON_BINDINGS


#endif // DUNE_XT_GRID_PROVIDER_PROVIDER_LIB_HH
