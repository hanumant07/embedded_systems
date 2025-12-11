#ifndef RING_BUFFER_TPP
#define RING_BUFFER_TPP

template <typename T>
RingBuffer<T>::RingBuffer(size_t size)
    : capacity(size), head(0), tail(0), full(false) {
  buffer = new T[capacity];
}

template <typename T> RingBuffer<T>::~RingBuffer() { delete[] buffer; }

template <typename T> bool RingBuffer<T>::push(const T &data) {
  if (full) {
    return false;
  }

  buffer[head] = data;
  head = (head + 1) % capacity;

  if (head == tail) {
    full = true;
  }

  return true;
}

template <typename T> bool RingBuffer<T>::pop(T *data) {
  if (isEmpty()) {
    return false;
  }

  *data = buffer[tail];
  tail = (tail + 1) % capacity;
  full = false;

  return true;
}

template <typename T> bool RingBuffer<T>::isEmpty() const {
  return (!full && (head == tail));
}

template <typename T> bool RingBuffer<T>::isFull() const { return full; }

#endif // RING_BUFFER_TPP
