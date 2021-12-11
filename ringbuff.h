#ifndef RINGBUFF_H
#define RINGBUFF_H

#include <cstddef>    /* size_t */
#include <vector>     /* vector */

class RingBuff
{
public:
  RingBuff(size_t buffsize);

  size_t xi, Ni, buffersize;
  double min, max;
  std::vector<double> V;
  size_t push(double data);
};

#endif // RINGBUFF_H
