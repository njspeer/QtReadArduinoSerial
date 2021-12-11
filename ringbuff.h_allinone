#ifndef RINGBUFF_H
#define RINGBUFF_H

#include <cstddef>
#include <vector>
#include <algorithm>    // std::minmax_element

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Simple ring-buffer class:
 * use it to make vectors of class T and size buffsize
 * I used std::vector class to gurentee contiguous memory
 * use push to add elements
 * when vector fills up, push will replace oldest index
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* class declaration */
template <class T>
class Vbuff
{
public:
  Vbuff(size_t buffsize);

  size_t xi, Ni, buffersize, min, max;
  std::vector<T> V;
  size_t push(T data);
};

/* constructor */
template <class T>
Vbuff<T>::Vbuff(size_t buffsize)
{
  buffersize = buffsize;
  V.reserve(buffsize);
  xi = 0;
  Ni = 0;
}

/* push method */
template <class T>
size_t Vbuff<T>::push(T data)
{
  if(V.size() < buffersize)
  {
    V.push_back(data);
    xi = Ni;
  }
  else
  {
    xi = Ni % buffersize;
    V[xi] = data;
  }

  const auto [xmin, xmax] = std::minmax_element(begin(V), end(V));
  min = (size_t)*xmin; max = (size_t)*xmax;
  return ++Ni;
}

#endif // RINGBUFF_H
