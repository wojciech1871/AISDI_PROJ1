/*
 * Wojciech Celej
 * 271248
 * AISDI
 * Projekt 1 - Liniowe
*/

#include <iostream>
#include <iomanip>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>

#include "Vector.h"
#include "LinkedList.h"

using listT=aisdi::LinkedList<std::string>;
using vectT=aisdi::LinkedList<std::string>;

std::chrono::system_clock::time_point tickTime()
{
  return std::chrono::system_clock::now();
}

size_t timePeriod(std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point end)
{
  return std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
}

template <class collectionType>
void insertEnd(collectionType& testCollection, size_t N)
{
  for(size_t i=0;i<N;i++)
  {
    testCollection.append("testEnd");
  }
}

template <class collectionType>
void insertBegin(collectionType& testCollection, size_t N)
{
  for(size_t i=0;i<N;i++)
  {
    testCollection.prepend("testBegin");
  }
}

template <class collectionType>
void eraseSecond(collectionType& testCollection, size_t N)
{
  for(size_t i=0;i<N;i++)
  {
    testCollection.erase(++testCollection.begin());
  }
}

void perfomTest(size_t collectSize)
{
  size_t* timeTab;
  timeTab=new size_t[6];
  std::chrono::system_clock::time_point start;
  std::chrono::system_clock::time_point end;
  listT testList;
  vectT testVector;
  std::cout <<"Times for "<<collectSize <<" elements:" <<std::endl;

  start=tickTime();
  insertEnd(testList,collectSize);
  end=tickTime();
  timeTab[0]=timePeriod(start,end);
  start=tickTime();
  insertBegin(testList,collectSize);
  end=tickTime();
  timeTab[1]=timePeriod(start,end);
  start=tickTime();
  eraseSecond(testList,collectSize);
  end=tickTime();
  timeTab[2]=timePeriod(start,end);

  start=tickTime();
  insertEnd(testVector,collectSize);
  end=tickTime();
  timeTab[3]=timePeriod(start,end);
  start=tickTime();
  insertBegin(testVector,collectSize);
  end=tickTime();
  timeTab[4]=timePeriod(start,end);
  start=tickTime();
  eraseSecond(testVector,collectSize);
  end=tickTime();
  timeTab[5]=timePeriod(start,end);

  std::cout <<std::setw(15) <<"" <<std::setw(25) <<"Insert at end" <<std::setw(25) <<"Insert at beginning" <<std::setw(25) <<"Erase second el." <<std::endl;
  std::cout <<std::setw(15) <<"LinkedList" <<std::setw(25) <<timeTab[0]<<std::setw(25) <<timeTab[1] <<std::setw(25) <<timeTab[2] <<std::endl;
  std::cout <<std::setw(15) <<"Vector" <<std::setw(25) <<timeTab[3]<<std::setw(25) <<timeTab[4] <<std::setw(25) <<timeTab[5] <<std::endl;
  delete[] timeTab;
}


int main()
{
  for(size_t i=10000;i<=1000000;i*=10)
  {
    perfomTest(i);
    std::cout <<std::endl;
  }
  return 0;
}
