// This file is part of the dune-xt-grid project:
//   https://github.com/dune-community/dune-xt-grid
// The copyright lies with the authors of this file (see below).
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
// Authors:
//   Felix Schindler (2013 - 2016)
//   Rene Milk       (2013 - 2015)
//   Tobias Leibner  (2014 - 2015)

#ifndef DUNE_XT_GRID_SEARCH_HH
#define DUNE_XT_GRID_SEARCH_HH

#include <vector>

#include <boost/range/iterator_range.hpp>

#include <dune/geometry/referenceelements.hh>

#include <dune/grid/common/gridview.hh>
#include <dune/grid/common/rangegenerators.hh>

#include <dune/xt/common/memory.hh>
#include <dune/xt/common/ranges.hh>

#include <dune/xt/grid/entity.hh>

namespace Dune {
namespace XT {
namespace Grid {

template <class GridViewType, int codim = 0>
class EntitySearchBase
{
  typedef typename GridViewType::Traits ViewTraits;
  static_assert(std::is_base_of<GridView<ViewTraits>, GridViewType>::value,
                "GridViewType has to be derived from GridView!");

public:
  typedef typename ViewTraits::template Codim<codim>::Entity EntityType;
  typedef typename EntityType::Geometry::LocalCoordinate LocalCoordinateType;
  typedef typename EntityType::Geometry::GlobalCoordinate GlobalCoordinateType;
  typedef std::vector<std::unique_ptr<EntityType>> EntityVectorType;
}; // class EntitySearchBase

template <int codim>
struct CheckInside
{
  template <class GeometryType, class GlobalCoordinateType>
  static bool check(const GeometryType& geometry, const GlobalCoordinateType& point)
  {
    if (codim == point.size()) {
      return Common::FloatCmp::eq(geometry.center(), point);
    } else {
      if (Common::FloatCmp::ne(geometry.global(geometry.local(point)), point))
        return false;
      const auto& refElement = reference_element(geometry);
      return refElement.checkInside(geometry.local(point));
    }
  }
};

template <>
struct CheckInside<0>
{
  template <class GeometryType, class GlobalCoordinateType>
  static bool check(const GeometryType& geometry, const GlobalCoordinateType& point)
  {
    const auto& refElement = reference_element(geometry);
    return refElement.checkInside(geometry.local(point));
  }
};

template <class GridViewType, int codim = 0>
class EntityInlevelSearch : public EntitySearchBase<GridViewType>
{
  typedef EntitySearchBase<GridViewType, codim> BaseType;

  typedef typename GridViewType::template Codim<codim>::Iterator IteratorType;

public:
  typedef typename BaseType::EntityVectorType EntityVectorType;

private:
  inline typename EntityVectorType::value_type check_add(const typename BaseType::EntityType& entity,
                                                         const typename BaseType::GlobalCoordinateType& point) const
  {
    const auto& geometry = entity.geometry();
    if (CheckInside<codim>::check(geometry, point)) {
      return Common::make_unique<typename BaseType::EntityType>(entity);
    }
    return nullptr;
  }

public:
  EntityInlevelSearch(const GridViewType& gridview)
    : gridview_(gridview)
    , it_last_(gridview_.template begin<codim>())
  {
  }

  template <class PointContainerType>
  EntityVectorType operator()(const PointContainerType& points)
  {
    const IteratorType begin = gridview_.template begin<codim>();
    const IteratorType end = gridview_.template end<codim>();
    EntityVectorType ret(points.size());
    typename EntityVectorType::size_type idx(0);
    for (const auto& point : points) {
      IteratorType it_current = it_last_;
      bool it_reset = true;
      typename EntityVectorType::value_type tmp_ptr(nullptr);
      for (; it_current != end; ++it_current) {
        if ((tmp_ptr = check_add(*it_current, point))) {
          ret[idx++] = std::move(tmp_ptr);
          tmp_ptr    = nullptr;
          it_reset   = false;
          it_last_   = it_current;
          break;
        }
      }
      if (!it_reset)
        continue;
      for (it_current = begin; it_current != it_last_; ++it_current) {
        if ((tmp_ptr = check_add(*it_current, point))) {
          ret[idx++] = std::move(tmp_ptr);
          tmp_ptr    = nullptr;
          it_reset   = false;
          it_last_   = it_current;
          break;
        }
      }
    }
    return ret;
  } // ... operator()

private:
  const GridViewType gridview_;
  IteratorType it_last_;
}; // class EntityInlevelSearch

template <class GridViewType>
class EntityHierarchicSearch : public EntitySearchBase<GridViewType>
{
  typedef EntitySearchBase<GridViewType> BaseType;

  const GridViewType gridview_;
  const int start_level_;

public:
  EntityHierarchicSearch(const GridViewType& gridview)
    : gridview_(gridview)
    , start_level_(0)
  {
  }

  typedef typename BaseType::EntityVectorType EntityVectorType;

  template <class PointContainerType>
  EntityVectorType operator()(const PointContainerType& points) const
  {
    auto level = std::min(gridview_.grid().maxLevel(), start_level_);
    auto range = elements(gridview_.grid().levelView(level));
    return process(points, range);
  }

private:
  template <class QuadpointContainerType, class RangeType>
  EntityVectorType process(const QuadpointContainerType& quad_points, const RangeType& range) const
  {
    EntityVectorType ret;

    for (const auto& my_ent : range) {
      const auto my_level    = my_ent.level();
      const auto& geometry   = my_ent.geometry();
      const auto& refElement = reference_element(geometry);
      for (const auto& point : quad_points) {
        if (refElement.checkInside(geometry.local(point))) {
          // if I cannot descend further add this entity even if it's not my view
          if (gridview_.grid().maxLevel() <= my_level || gridview_.contains(my_ent)) {
            ret.emplace_back(my_ent);
          } else {
            const auto h_end   = my_ent.hend(my_level + 1);
            const auto h_begin = my_ent.hbegin(my_level + 1);
            const auto h_range = boost::make_iterator_range(h_begin, h_end);
            const auto kids = process(QuadpointContainerType(1, point), h_range);
            ret.insert(ret.end(), kids.begin(), kids.end());
          }
        }
      }
    }
    return ret;
  }
}; // class EntityHierarchicSearch

template <class GV, int codim = 0>
EntityInlevelSearch<GV, codim> make_entity_in_level_search(const GV& grid_view)
{
  return EntityInlevelSearch<GV, codim>(grid_view);
}

template <class GV>
EntityHierarchicSearch<GV> make_entity_hierarchic_search(const GV& grid_view)
{
  return EntityHierarchicSearch<GV>(grid_view);
}

} // namespace Grid
} // namespace XT
} // namespace Dune

#endif // DUNE_XT_GRID_SEARCH_HH