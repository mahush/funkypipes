//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_MAKE_PIPE_HPP
#define FUNKYPIPES_MAKE_PIPE_HPP

#include "funkypipes/details/make_raw_pipe.hpp"
#include "funkypipes/details/make_skippable.hpp"
#include "funkypipes/details/with_empty_tuple_result_as_void.hpp"
#include "funkypipes/details/with_non_single_args_tupled.hpp"
#include "funkypipes/details/with_signature_check.hpp"
#include "funkypipes/details/with_tuple_arg_unpacked.hpp"
#include "funkypipes/details/with_void_result_as_empty_tuple.hpp"

namespace funkypipes {

template <typename... TFns>
auto makePipe(TFns&&... fns) {
  using namespace details;
  return withNonSingleArgsTupled(withEmptyTupleResultAsVoid(
      makeRawPipe(withVoidResultAsEmptyTuple(withTupleArgUnpacked(withSignatureCheck(std::forward<TFns>(fns))))...)));
}

template <typename TFn>
auto andThen(TFn&& fn) {
  using namespace details;
  return makeSkippable(withVoidResultAsEmptyTuple(withTupleArgUnpacked(withSignatureCheck(std::forward<TFn>(fn)))));
}
}  // namespace funkypipes

#endif  // FUNKYPIPES_MAKE_PIPE_HPP
