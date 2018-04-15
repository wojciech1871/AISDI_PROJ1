#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#define INIT_CAPACITY 10

namespace aisdi
{

template <typename Type>
class Vector
{
  size_t capacity;
  size_t numberOfElements;
  Type* tabPtr;
  Type* guardPtr;

  void move(size_t index)
  {
    for(size_t i=numberOfElements;i>index;i--)
    {
      tabPtr[i]=tabPtr[i-1];
    }
  }
  void move(size_t index, size_t distance)
  {
    for(size_t i=index;i<numberOfElements;i++)
    {
      tabPtr[i-distance]=tabPtr[i];
    }
  }
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  Vector()
  : capacity(INIT_CAPACITY), numberOfElements(0), tabPtr(new Type[INIT_CAPACITY])
  {
    guardPtr=tabPtr;
  }

  Vector(std::initializer_list<Type> l)
  : Vector()
  {
    for(auto it=l.begin(); it!=l.end(); ++it)
    {
      append(*it);
    }
  }

  Vector(const Vector& other)
  : Vector()
  {
    for(auto it=other.begin(); it!=other.end(); ++it)
    {
      append(*it);
    }
  }

  Vector(Vector&& other)
  {
    this->capacity=other.capacity;
    this->numberOfElements=other.numberOfElements;
    this->tabPtr=other.tabPtr;
    this->guardPtr=other.guardPtr;

    other.capacity=0;
    other.numberOfElements=0;
    other.tabPtr=nullptr;
    other.guardPtr=nullptr;
  }

  ~Vector()
  {
    delete[] tabPtr;
  }

  Vector& operator=(const Vector& other)
  {
    if(this!=&other)
    {
      delete[] tabPtr;
      this->numberOfElements=0;
      this->capacity=other.capacity;
      this->tabPtr=new Type[this->capacity];
      this->guardPtr=this->tabPtr;
      for(auto it=other.begin(); it!=other.end(); ++it)
      {
        append(*it);
      }
    }
    return *this;
  }

  Vector& operator=(Vector&& other)
  {
    if(this!=&other)
    {
      delete[] tabPtr;
      this->numberOfElements=other.numberOfElements;
      this->capacity=other.capacity;
      this->tabPtr=other.tabPtr;
      this->guardPtr=other.guardPtr;

      other.numberOfElements=0;
      other.capacity=0;
      other.tabPtr=nullptr;
      other.guardPtr=nullptr;
    }
    return *this;
  }

  bool isEmpty() const
  {
    if(numberOfElements)
    {
      return false;
    }
    return true;
  }

  size_type getSize() const
  {
    return numberOfElements;
  }

  void append(const Type& item)
  {
    insert(cend(),item);
    return;
  }

  void prepend(const Type& item)
  {
    insert(cbegin(),item);
    return;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    size_t index;
    index=static_cast<size_t>(insertPosition.getPtr()-tabPtr);
    if(capacity==numberOfElements)
    {
      Type* pom=new Type[capacity+INIT_CAPACITY];
      for(size_t i=0;i<capacity;i++)
      {
        pom[i]=tabPtr[i];
      }
      delete[] tabPtr;
      tabPtr=pom;
      guardPtr=tabPtr+numberOfElements;
      capacity=capacity+INIT_CAPACITY;
    }
    move(index);
    tabPtr[index]=item;
    numberOfElements++;
    guardPtr++;
  }

  Type popFirst()
  {
    if(numberOfElements==0)
    {
      throw std::out_of_range("Empty collection");
    }
    Type temp=tabPtr[0];
    erase(cbegin());
    return temp;
  }

  Type popLast()
  {
    if(numberOfElements==0)
    {
      throw std::out_of_range("Empty collection");
    }
    Type temp=*(--cend());
    erase(--cend());
    return temp;
  }

  void erase(const const_iterator& possition)
  {
    if(possition==cend())
    {
       throw std::out_of_range("Cannot erase element form out of the list");
    }
    size_t index;
    index=static_cast<size_t>(possition.getPtr()-tabPtr);
    move(index+1,1);
    numberOfElements--;
    guardPtr--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    size_t index;
    size_t distance;
    index=static_cast<size_t>(lastExcluded.getPtr()-tabPtr);
    distance=static_cast<size_t>(lastExcluded.getPtr()-firstIncluded.getPtr());
    move(index,distance);
    numberOfElements-=distance;
    guardPtr-=distance;
  }

  iterator begin()
  {
    return Iterator(tabPtr,this);
  }

  iterator end()
  {
    return Iterator(guardPtr,this);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(tabPtr,this);
  }

  const_iterator cend() const
  {
    return ConstIterator(guardPtr,this);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
  Type* currentPtr;
  const Vector<Type>* vectPtr;
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

  explicit ConstIterator(Type* element, const Vector<Type>* vectorPtr)
  : currentPtr(element), vectPtr(vectorPtr)
  {}

  reference operator*() const
  {
    if(currentPtr==vectPtr->guardPtr)
    {
      throw std::out_of_range("Cannot dereference");
    }
    return *currentPtr;
  }

  ConstIterator& operator++()
  {
    if(currentPtr==vectPtr->guardPtr)
    {
      throw std::out_of_range("Cannot dereference");
    }
    currentPtr++;
    return *this;
  }

  ConstIterator operator++(int)
  {
    if(currentPtr==vectPtr->guardPtr)
    {
      throw std::out_of_range("Cannot dereference");
    }
    ConstIterator temp =*this;
    currentPtr++;
    return temp;
  }

  ConstIterator& operator--()
  {
    if(currentPtr==vectPtr->tabPtr)
    {
      throw std::out_of_range("Cannot dereference");
    }
    currentPtr--;
    return *this;
  }

  ConstIterator operator--(int)
  {
    if(currentPtr==vectPtr->tabPtr)
    {
      throw std::out_of_range("Cannot dereference");
    }
    ConstIterator temp =*this;
    currentPtr--;
    return temp;
  }

  ConstIterator operator+(difference_type d) const
  {
    ConstIterator temp(*this);
    temp.currentPtr+=d;
    return temp;
  }

  ConstIterator operator-(difference_type d) const
  {
    ConstIterator temp(*this);
    temp.currentPtr-=d;
    return temp;
  }

  bool operator==(const ConstIterator& other) const
  {
    return (currentPtr==other.currentPtr&&vectPtr==other.vectPtr);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return (currentPtr!=other.currentPtr||vectPtr!=other.vectPtr);
  }
  Type* getPtr() const
  {
    return currentPtr;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator(Type* element, Vector<Type>* vectorPtr)
  : ConstIterator(element,vectorPtr)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_VECTOR_H
