#include "ringbuff.h"
#include <QDebug>

/* constructor */
RingBuff::RingBuff(size_t buffsize)
{
  buffersize = buffsize;
  V.reserve(buffsize);
  xi = 0;
  Ni = 0;
}

/* push method */
size_t RingBuff::push(double data)
{
  /* use xi to track the Nth element */
  if(Ni < buffersize)
  {
    xi = Ni;
    V.push_back(data);
    if(xi==0)
    {
      min = data;
      max = data;
    }
    else
    {
      min = std::min(min, data);
      max = std::max(max, data);
    }
  }
  else
  {
    /* xi rolls over when it reaches the end of the vector */
    xi = Ni % buffersize;
    V.at(xi) = data;

    /* once data rolls over, we calculate the minmax the hard way */
    const auto minmax = std::minmax_element(begin(V), end(V));
    min = *minmax.first;
    max = *minmax.second;
  }

  /* calculate the min and max each time a new point is added */
  return ++Ni;
}
