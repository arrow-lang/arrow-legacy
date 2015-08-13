// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PTR_H
#define ARROW_PTR_H 1

#include <cstddef>
#include <typeinfo>
#include <algorithm>

namespace arrow {

template <typename T>
class Ref {
 public:
  Ref() noexcept : Ref(nullptr) {
  }

  Ref(std::nullptr_t) noexcept
      : _d(nullptr), _refs(nullptr), _is_undefined(false) {
  }

  Ref(T* raw) noexcept : _d(raw), _refs(nullptr), _is_undefined(false) {
    _refs = new std::size_t(1);
  }

  template <typename U>
  Ref(const Ref<U>& other) noexcept
      : _d(dynamic_cast<T*>(other._d)),
        _refs(other._refs),
        _is_undefined(other._is_undefined) {
    if (other._refs) { (*other._refs) += 1; }
  }

  Ref(const Ref& other) noexcept
      : _d(other._d),
        _refs(other._refs),
        _is_undefined(other._is_undefined) {
    if (other._refs) { (*other._refs) += 1; }
  }

  Ref(Ref&& other) noexcept : Ref(nullptr) {
    swap(*this, other);
  }

  inline operator bool() const noexcept {
    return _d != nullptr && !_is_undefined;
  }

  template <typename U>
  bool is() const noexcept {
    return dynamic_cast<U*>(_d) != nullptr;
  }

  template <typename U>
  Ref<U> as() {
    return Ref<U>(dynamic_cast<U*>(_d), _refs, _is_undefined);
  }

  virtual ~Ref() noexcept {
    if (_d) {
      if ((*_refs) <= 1) {
        delete _d;
        delete _refs;
      } else {
        (*_refs) -= 1;
      }
    }
  }

  friend void swap(Ref& a, Ref& b) noexcept {
    using std::swap;

    swap(a._d, b._d);
    swap(a._refs, b._refs);
    swap(a._is_undefined, b._is_undefined);
  }

  friend bool operator==(const Ref& a, std::nullptr_t) noexcept {
    return !bool(a);
  }

  friend bool operator==(std::nullptr_t, const Ref& a) noexcept {
    return !bool(a);
  }

  friend bool operator!=(const Ref& a, std::nullptr_t) noexcept {
    return bool(a);
  }

  friend bool operator!=(std::nullptr_t, const Ref& a) noexcept {
    return bool(a);
  }

  Ref& operator =(Ref other) noexcept {
    swap(*this, other);
    return *this;
  }

  T* get() noexcept {
    return _d ? _d : nullptr;
  }

  const T* get() const noexcept {
    return _d ? _d : nullptr;
  }

  T* operator ->() noexcept {
    return get();
  }

  const T* operator ->() const noexcept {
    return get();
  }

  T& operator *() noexcept {
    return *_d;
  }

  const T& operator *() const noexcept {
    return *_d;
  }

  bool is_undef() const noexcept {
    return _is_undefined;
  }

 private:
  Ref(T* raw, std::size_t* refs, bool is_undefined) noexcept
      : _d(raw), _refs(refs), _is_undefined(is_undefined) {
    if (_refs) { (*_refs) += 1; }
  }

  template <typename U>
  friend Ref<U> make_undef();

  template <typename U>
  friend class Ref;

  T* _d;
  std::size_t* _refs;
  bool _is_undefined;
};

/// An `undefined` reference. This is different than a `null` reference.  A
/// `null` reference means something internal broke. An `undefined` reference
/// means that something broke but we detected it and reported an error.
template <typename T>
Ref<T> make_undef() {
  Ref<T> ref(nullptr);
  ref._is_undefined = true;
  return ref;
}

}  // namespace arrow

namespace std {

template <typename T>
struct hash<arrow::Ref<T>> {
  std::size_t operator()(const arrow::Ref<T>& k) const {
    return reinterpret_cast<std::size_t>(k.get());
  }
};

}  // namespace std

#endif  // ARROW_PTR_H
