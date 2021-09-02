//
// Created by asuarez on 8/5/21.
//

#ifndef XX90_MANAGER_CMAKE_SPIN_POLICY_HPP
#define XX90_MANAGER_CMAKE_SPIN_POLICY_HPP

#include "outcome-basic.hpp"

namespace outcome = OUTCOME_V2_NAMESPACE;

struct spin_policy : outcome::policy::base {
  template<class Impl>
  static constexpr void wide_value_check(Impl &&self) {
    if (!base::_has_value(std::forward<Impl>(self)))
      while(true) {}
  }

  template<class Impl>
  static constexpr void wide_error_check(Impl &&self) {
    if (!base::_has_error(std::forward<Impl>(self)))
      while(true) {}
  }

  template<class Impl>
  static constexpr void wide_exception_check(Impl &&self) {
    if (!base::_has_exception(std::forward<Impl>(self)))
      while(true) {}
  }
};

template<class R, class S>
using result = outcome::basic_result<R, S, spin_policy>;


#endif //XX90_MANAGER_CMAKE_SPIN_POLICY_HPP
