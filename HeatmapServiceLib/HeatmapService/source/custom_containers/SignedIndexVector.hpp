///////////////////////////////////////////////////////////////////////////
// SignedIndexVector.h: A helper std::vector-like class that is able to be 
//  indexed by both positive as well as negative values.
//  The signed index vector is made to feel "seemingly infinite" as when 
//  it's accessed out of range via the non-const operator[] it dynamically 
//  allocates memory to hold the accessed index, or returns default
//  but valid values when read out of bounds using the get_at method.
// Written by: Pedro Engana (http://pedroengana.com) 
///////////////////////////////////////////////////////////////////////////
#pragma once

// For use of the std::allocator
#include <memory>

// Boost headers for Serialization
#include <boost\serialization\access.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\archive\binary_iarchive.hpp>

namespace heatmap_service
{

  template <typename T>
  class SignedIndexVector
  {
  public:
    using iterator = T*;
    using const_iterator = const T*;
    using siv_size = size_t;

  private:
    std::allocator<T> alloc;

    // Start of allocated memory
    iterator mem_begin_;

    // Beggining of initialized values
    iterator begin_;

    // Result of operator[0]
    iterator index_zero_;

    // End of initialized values
    iterator end_;

    // End of allocated memory
    iterator mem_end_;

  public:
    SignedIndexVector() { create(); }

    explicit SignedIndexVector(siv_size pre_allocate, const T& initialize_with = T()){ create(pre_allocate, initialize_with); }

    SignedIndexVector(const SignedIndexVector& copy) { create(copy.begin(), copy.end(), copy.index_zero()); }

    SignedIndexVector& operator=(const SignedIndexVector& copy) {
      if (this != &copy)
      {
        destroy();
        create(copy.begin(), copy.end(), copy.index_zero());
      }
      return *this;
    }

    ~SignedIndexVector(){ destroy(); }

    // -- Iterators
    iterator begin(){ return begin_; }
    const_iterator begin() const { return begin_; }

    iterator index_zero(){ return index_zero_; }
    const_iterator index_zero() const { return index_zero_; }

    iterator end(){ return end_; }
    const_iterator end() const { return end_; }

    // -- Getters
    int lowest_index() const { return begin_ - index_zero_; }
    siv_size size() const { return end_ - begin_; }
    siv_size allocation_size() const { return mem_end_ - mem_begin_; }

    // -- Operators
    T& operator[](int index){
      // If the index exceeds the allocated memory, then grow allocation size to match the required index
      if (index_zero_ + index >= mem_end_ || index_zero_ + index <= mem_begin_)
        // Grow takes a size of (index+2)*2 because grow increases memory for positive indexes and negative indexes equally
        // +2 is added to the index to slightly overshoot required memory to acount for mem_end being a non-initialized value
        grow((abs(index) + 1 )*2);

      // After memory allocation is guaranteed to hold the needed amount, we initialize all positions from current begin/end
      // to the required index (odds are these values will be needed later anyways)
      if (index + index_zero_ >= end_ || index + index_zero_ < begin_)
        initialize_to_index(index);

      return *(index + index_zero_);
    }

    // const operator [] throws out of range as one would expect from a normal vector.
    const T& operator[] (int index) const {
      if (index + index_zero_ >= end_ || index + index_zero_ < begin_)
        throw std::out_of_range("Signed Index Vector Error: const operator[], index out of range");

      return *(index + index_zero_);
    }

    // Get_at works similarly to const operator[], but it returns a default value of T instead of an 
    // out-of-range when non-existent indexes are queried. Usefull to keep the "seemingly infinite" abstraction
    T get_at(int index) const {
      if (index + index_zero_ >= end_ || index + index_zero_ < begin_)
        return T();

      return *(index + index_zero_);
    }

    // -- push_back & push_front
    // As one would expect from similar containers found in the STL
    void push_back(const T& value){
      if (end_ == mem_end_)
        grow( (siv_size)((mem_end_ - mem_begin_)*2) );

      std::uninitialized_fill(end_, end_ + 1, value);
      ++end_;
    }

    void push_front(const T& value){
      if (begin_ == mem_begin_)
        grow( (siv_size)((mem_end_ - mem_begin_)*2) );

      std::uninitialized_fill(begin_ - 1, begin_, value);
      --begin_;
    }

    // -- Cleaners
    // Deletes all data but maintains allocation
    void clear(){
      iterator it = end_;
      while (it != begin_)
        alloc.destroy(--it);

      begin_ = end_ = index_zero_;
    }

    // Deletes all data and frees memory
    void clean(){
      destroy();
      create();
    }

  private:
    void create(){ mem_begin_ = begin_ = index_zero_ = end_ = mem_end_ = nullptr; }
    // Allocates uninitialized memory of the requested size
    void create(siv_size size, const T& init_value = T()){
      begin_ = mem_begin_ = alloc.allocate(size);
      index_zero_ = begin_ + size / 2;
      end_ = mem_end_ = begin_ + size;
      std::uninitialized_fill(begin_, mem_end_, init_value);
    }
    // Allocates and initializes memory copying contents from [begin, end[
    void create(const_iterator begin, const_iterator end, const_iterator zero_index){
      if (begin == end){
        create();
        return;
      }
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

    // Initializes memory to init_value. Used to guarantee no gaps between initialized memory
    void initialize_to_index(int index, const T& init_value = T()){
      if (index + index_zero_ >= end_) {
        std::uninitialized_fill(end_, index_zero_ + index + 1, init_value);
        end_ = index_zero_ + index + 1;
      }
      else if (index + index_zero_ < begin_) {
        std::uninitialized_fill(index + index_zero_, begin_, init_value);
        begin_ = index_zero_ + index;
      }
    }

    // Reallocates memory to fit a new needed size
    void grow(siv_size needed_size = 0){
      siv_size curr_size = allocation_size();
      siv_size new_size = (siv_size)((mem_end_ - mem_begin_)*1.5 > 2 ? (mem_end_ - mem_begin_)*1.5 : 2);

      // Grows "new_size" by 1.5 until we reach a size that can fit our needs
      while (new_size < needed_size)
        new_size = (siv_size)(new_size*1.5);

      iterator new_mem_begin = alloc.allocate(new_size);

      iterator new_index_zero = new_mem_begin + new_size / 2;
      iterator new_begin = new_index_zero + lowest_index();
      siv_size mem_b_to_b = new_begin - new_mem_begin;
      iterator new_end = begin_ ? std::uninitialized_copy(begin_, end_, new_begin) : new_begin;

      destroy();

      mem_begin_ = new_mem_begin;
      begin_ = new_begin;
      index_zero_ = new_index_zero;
      end_ = new_end;
      mem_end_ = new_mem_begin + new_size;
    }

    // Boost serialization methods
    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
      siv_size current_size = size();
      int negative_index = lowest_index();

      // Serialize descriptive variables
      ar & current_size;
      ar & negative_index;

      // Serialize all vector values
      for (const_iterator i = begin_; i != end_; ++i)
        ar & *i;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
      // Ensures the signed index vector is cleaned and deallocated before loading the serialized values
      clean();

      // Load descriptive variables
      siv_size current_size;
      int negative_index;

      ar & current_size;
      ar & negative_index;

      if (current_size == 0)
        return;

      // Allocate and initialize vector
      create(current_size);
      for (iterator i = begin_; i != end_; ++i)
        ar & *i;

      index_zero_ = begin_ + (-1 * negative_index);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
  };

}