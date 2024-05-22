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
#include "funkypipes/details/make_signature_checking.hpp"
#include "funkypipes/details/make_skippable.hpp"
#include "funkypipes/details/make_tuple_packing.hpp"
#include "funkypipes/details/make_tuple_unpacking.hpp"

namespace funkypipes {

template <typename... TFns>
auto makePipe(TFns&&... fns) {
  using namespace details;
  return makeTuplePacking(makeRawPipe(makeTupleUnpacking(makeSignatureChecking(std::forward<TFns>(fns)))...));
}

template <typename TFn>
auto andThen(TFn&& fn) {
  using namespace details;
  return makeSkippable(makeTupleUnpacking(makeSignatureChecking(std::forward<TFn>(fn))));
}
}  // namespace funkypipes

#endif  // FUNKYPIPES_MAKE_PIPE_HPP
