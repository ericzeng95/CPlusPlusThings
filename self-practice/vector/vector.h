#include <cstddef>
#include <new>
#include <stdexcept>
#include <vector>
template <typename T> class Vector {
public:
  // ITERATOR DEFINITION
  using iterator = T *;
  using const_iterator = const T *;

  // CONSTRUCTORS
  Vector();

  Vector(size_t size);

  Vector(size_t size, T &value);

  Vector(const Vector &other);

  Vector &operator=(const Vector &other);

  Vector(const Vector &&other);

  Vector &operator=(const Vector &&other);

  void reserve(size_t new_capacity);

  void emplace_back(T &&value);

  void clear() noexcept;

  void swap(const Vector &other);

  // ELEMENTS
  void push_back(const T &value);

  void push_back(const T &&value);

  template <typename... Args> void emplace_back(Args &&... args);

  void pop();

  // ITERATORS
  iterator begin() { return d_data; }
  iterator end() { return d_data + d_size; }
  const_iterator begin() const { return d_data; }
  const_iterator end() const { return d_data + d_size; }
  const_iterator cbegin() const { return d_data; }
  const_iterator cend() const { return d_data + d_size; }

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
  size_t d_size;
  size_t d_capacity;

  // MEMORY ALLOCATION
  T *allocate(size_t capacity) {
    return static_cast<T *>(::operator new(capacity * sizeof(T)));
  }

  void deallocate() {
    if (d_data) {
        ::operator delete[](d_data);
    }
  }
};

template <typename T> Vector<T>::Vector() {}
