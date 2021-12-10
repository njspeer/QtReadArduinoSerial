#include "ringbuff.cpp"
#include <iostream>
using namespace std;

template <class T> 
void printV(std::vector<T> V)
{
  int Ni = V.size();
  if(Ni==0){return;}
  cout << V[0];
  for(int i=1;i<Ni;++i){cout << ", " << V[i];}
  cout << endl;
}

int main()
{
  Vbuff<double> b = Vbuff<double>(20);

  for(int i = 0; i < 25; ++i)
  {
    b.push(i);
  }
  printV(b.V);

  cout << b.min << ", " << b.max << endl;
}