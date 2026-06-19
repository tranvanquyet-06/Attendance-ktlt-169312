#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

template <typename T>
class DynamicArray {
private:
    T* data;
    int _size;
    int _capacity;

    void expand(int newCapacity) {
        T* newData = new T[newCapacity];
        for (int i = 0; i < _size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        _capacity = newCapacity;
    }

public:
    DynamicArray() : data(nullptr), _size(0), _capacity(0) {}

    DynamicArray(const DynamicArray& other) : data(nullptr), _size(0), _capacity(0) {
        if (other._size > 0) {
            _capacity = other._capacity;
            _size = other._size;
            data = new T[_capacity];
            for (int i = 0; i < _size; i++) {
                data[i] = other.data[i];
            }
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            data = nullptr;
            _size = 0;
            _capacity = 0;
            if (other._size > 0) {
                _capacity = other._capacity;
                _size = other._size;
                data = new T[_capacity];
                for (int i = 0; i < _size; i++) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
    }

   void push_back(const T& item) {
       if (_size >= _capacity) {
 int newCap = (_capacity == 0) ? 4 : _capacity * 2;
           expand(newCap);
        }
        data[_size++] = item;
 }

  void removeAt(int index) {
      if (index < 0 || index >= _size) return;
        for (int i=index; i<_size-1; i++) {
         data[i] = data[i+1];
     }
      _size--;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    int size() const { return _size; }

    bool empty() const { return _size == 0; }

    void clear() {
        _size = 0;
    }
};

#endif
