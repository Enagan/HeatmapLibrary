#include <memory>

template <typename T>
class SignedIndexVector
{
public:
  using iterator = T*;
  using const_iterator = const T*;
  using siv_size = size_t;

private:
  std::allocator<T> alloc;
  iterator mem_begin_;
  iterator begin_;
  iterator index_zero_;
  iterator end_;
  iterator mem_end_;

public:
  SignedIndexVector() { create(); }
  explicit SignedIndexVector(siv_size pre_allocate, const T& initialize_with = T()){ create(pre_allocate, initialize_with); }
  SignedIndexVector(const SignedIndexVector& copy) { create(copy.cbegin(), copy.cend(), copy.cindex_zero()); }
  SignedIndexVector& operator=(const SignedIndexVector& copy) {
    if (this != &copy)
    {
      destroy();
      create(copy.cbegin(), copy.cend(), copy.cindex_zero());
    }
    return *this;
  }
  ~SignedIndexVector(){ destroy(); }

  iterator begin(){ return begin_; }
  const_iterator cbegin() const { return begin_; }

  iterator index_zero(){ return index_zero_; }
  const_iterator cindex_zero() const { return index_zero_; }

  iterator end(){ return end_; }
  const_iterator cend() const { return end_; }

  int lowest_index() const { return begin_ - index_zero_; }
  siv_size size() const { return end_ - begin_; }
  siv_size allocation_size() const { return mem_end_ - mem_begin_; }

  T& operator[](int index){
    while (index_zero_ + index >= mem_end_ || index_zero_ + index <= mem_begin_)
      grow();

    if (index + index_zero_ >= end_ || index + index_zero_ <= begin_)
      initialize_to_index(index);

    return *(index + index_zero_);
  }

  const T& operator[] (int index) const {
    while (index_zero_ + index >= mem_end_ || index_zero_ + index <= mem_begin_)
      grow();

    if (index + index_zero_ >= end_ || index + index_zero_ <= begin_)
      initialize_to_index(index);

    return *(index + index_zero_);
  }

  T get_at(int index) const {
    if (index + index_zero_ > end_ || index + index_zero_ < begin_)
      return T();

    return *(index + index_zero_);
  }

  void push_back(const T& value){
    if (end_ == mem_end_)
      grow();

    *(end_++) = value;
  }

  void push_front(const T& value){
    if (begin_ == mem_begin_)
      grow();

    *(--begin_) = value;
  }

  void clear(){
    iterator it = end_;
    while (it != begin_)
      alloc.destroy(--it);

    begin_ = end_ = index_zero_;
  }

  void clean(){
    destroy();
    create();
  }

private:
  void create(){ mem_begin_ = begin_ = index_zero_ = end_ = mem_end_ = nullptr; }
  void create(siv_size size, const T& init_value){
    begin_ = mem_begin_ = alloc.allocate(size);
    index_zero_ = begin_ + size / 2;
    end_ = mem_end_ = begin_ + size;
    std::uninitialized_fill(begin_, mem_end_, init_value);
  }
  void create(const_iterator begin, const_iterator end, const_iterator zero_index){
    siv_size dist_to_zero = zero_index - begin;
    begin_ = mem_begin_ = alloc.allocate(end - begin);
    index_zero_ = begin_ + dist_to_zero;
    end_ = mem_end_ = std::uninitialized_copy(begin, end, begin_);
  }
  void destroy(){
    if (begin_) {
      iterator it = end_;
      while (it != begin_)
        alloc.destroy(--it);

      alloc.deallocate(mem_begin_, mem_end_ - mem_begin_);
    }

    mem_begin_ = begin_ = index_zero_ = end_ = mem_end_ = nullptr;
  }

  void initialize_to_index(int index, const T& init_value = T()){
    if (index + index_zero_ >= end_) {
      std::uninitialized_fill(end_, index_zero_ + index + 1, init_value);
      end_ = index_zero_ + index + 1;
    }
    else if (index + index_zero_ <= begin_) {
      std::uninitialized_fill(index + index_zero_, begin_, init_value);
      begin_ = index_zero_ + index;
    }
  }

  void grow(){
    siv_size new_size = (siv_size)((mem_end_ - mem_begin_)*1.5 > 0 ? (mem_end_ - mem_begin_)*1.5 : 2);
    iterator new_mem_begin = alloc.allocate(new_size);

    iterator new_index_zero = new_mem_begin + new_size / 2;
    iterator new_begin = new_index_zero + lowest_index();
    iterator new_end = begin_ ? std::uninitialized_copy(begin_, end_, new_begin) : new_begin;

    destroy();

    mem_begin_ = new_mem_begin;
    begin_ = new_begin;
    index_zero_ = new_index_zero;
    end_ = new_end;
    mem_end_ = new_mem_begin + new_size;
  }
};