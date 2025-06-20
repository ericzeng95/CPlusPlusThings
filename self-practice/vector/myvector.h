#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <new>
#include <stdexcept>
#include <utility>
#include <vector>
template <typename T> class Vector {
public:
  // ITERATOR DEFINITION
  using iterator = T *;
  using const_iterator = const T *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // CONSTRUCTORS
  Vector() = default;

  ~Vector() {
    clear();
    deallocate();
  }

  Vector(size_t capacity) { reserve(capacity); }

  Vector(size_t size, T &value);

  Vector(const Vector &other);

  Vector &operator=(const Vector &other);

  Vector(Vector &&other) noexcept
      : d_data(other.d_data), d_size(other.d_size),
        d_capacity(other.d_capacity) {
    other.d_data = nullptr;
    other.d_size = 0;
    other.d_capacity = 0;
  }

  Vector &operator=(Vector &&other) {
    if (this != &other) {
      clear();
      deallocate();
      swap(other);
    }
    return *this;
  }

  void reserve(size_t new_capacity) {
    if (new_capacity <= d_capacity)
      return;
    reallocate(new_capacity);
  }

  void clear() noexcept {
    for (size_t i = 0; i < d_size; ++i) {
      d_data[i].~T();
    }
    d_size = 0;
  }

  void swap(Vector &other) noexcept {
    std::swap(d_data, other.d_data);
    std::swap(d_size, other.d_size);
    std::swap(d_capacity, other.d_capacity);
  }

  // ELEMENTS
  void push_back(const T &value) { emplace_back(value); }

  void push_back(T &&value) { emplace_back(std::move(value)); }

  template <typename... Args> T &emplace_back(Args &&... args) {
    if (d_size == d_capacity) {
      reserve(d_capacity == 0 ? 1 : d_capacity * 2);
    }

    new (d_data + d_size) T(std::forward<Args>(args)...);
    return d_data[d_size++];
  }

  void pop() {
    if (d_size > 0) {
      d_data[d_size - 1].~T();
      --d_size;
    }
  }

  // ITERATORS
  iterator begin() { return d_data; }
  iterator end() { return d_data + d_size; }
  const_iterator begin() const { return d_data; }
  const_iterator end() const { return d_data + d_size; }
  const_iterator cbegin() const { return d_data; }
  const_iterator cend() const { return d_data + d_size; }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  iterator insert(const_iterator pos, const T &value);
  iterator insert(const_iterator pos, const T &&value);

  iterator erase(const_iterator pos);

  // SIZES
  size_t size() const { return d_size; }
  size_t capacity() const { return d_capacity; }
  bool empty() const { return d_size == 0; }

  // ACCESSORS
  T &operator[](size_t index) { return d_data[index]; }
  const T &operator[](size_t index) const { return d_data[index]; }

  T &at(size_t index) {
    if (index >= d_size) {
      throw std::out_of_range("Vector::at out of range");
    }
    return d_data[index];
  }

  const T &at(size_t index) const {
    if (index >= d_size) {
      throw std::out_of_range("Vector::at out of range");
    }
    return d_data[index];
  }

  T &back() noexcept { return d_data[d_size - 1]; }
  const T &back() const noexcept { return d_data[d_size - 1]; }

private:
  T *d_data = nullptr;
  size_t d_size{0};
  size_t d_capacity{0};

  // MEMORY ALLOCATION
  T *allocate(size_t capacity) {
    return static_cast<T *>(::operator new(capacity * sizeof(T)));
  }

  void deallocate() {
    if (d_data) {
      ::operator delete(d_data);
      d_data = nullptr;
      d_capacity = 0;
    }
  }

  void reallocate(size_t new_capacity) {
    // getting new memory
    T *new_data = allocate(new_capacity);

    // move construction
    size_t new_size = 0;
    try {
      for (; new_size < d_size; ++new_size) {
        new (new_data + new_size) T(std::move_if_noexcept(d_data[new_size]));
      }
    } catch (...) {
      for (size_t i = 0; i < new_size; ++i) {
        new_data[i].~T();
      }
      ::operator delete(new_data);
      throw;
    }

    // delete old and clear memory
    clear();
    deallocate();

    d_data = new_data;
    d_size = new_size;
    d_capacity = new_capacity;
  }
};

template <typename T> void swap(Vector<T> &lhs, Vector<T> &rhs) {
  lhs.swap(rhs);
}