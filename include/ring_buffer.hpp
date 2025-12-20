#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <stddef.h> // for size_t

template <typename T> class RingBuffer {
public:
  explicit RingBuffer(size_t size);
  ~RingBuffer();

  bool push(const T &data);
  bool pop(T *data);
  bool isEmpty() const;
  bool isFull() const;

private:
  T *buffer;
  size_t capacity;
  size_t head;
  size_t tail;
  bool full;
};

#include "ring_buffer.tpp"

#endif // RING_BUFFER_HPP
