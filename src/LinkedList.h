#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
  struct Node
  {
    Type value;
    Node* nextPtr;
    Node* prevPtr;

    Node(): nextPtr(nullptr), prevPtr(nullptr)
    {}
    Node(const Type& value)
    : value(value), nextPtr(nullptr), prevPtr(nullptr)
    {}
  };
  Node* head;
  Node* tail;
  size_t numberOfNodes;

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

  LinkedList()
  : numberOfNodes(0)
  {
    head=tail=new Node();
  }

  LinkedList(std::initializer_list<Type> l)
  : LinkedList()
  {
    for(auto it=l.begin(); it!=l.end(); ++it)
    {
      append(*it);
    }
  }

  LinkedList(const LinkedList& other)
  : LinkedList()
  {
    for(auto it=other.begin(); it!=other.end(); ++it)
    {
      append(*it);
    }
  }

  LinkedList(LinkedList&& other)
  {
    this->numberOfNodes=other.numberOfNodes;
    this->head=other.head;
    this->tail=other.tail;

    other.numberOfNodes=0;
    other.head=nullptr;
    other.tail=nullptr;
  }

  ~LinkedList()
  {
    erase(cbegin(), cend());
    delete head;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    if(this!=&other)
    {
      this->erase(cbegin(),cend());
      delete head;
      this->numberOfNodes=other.numberOfNodes;
      this->head=this->tail=new Node();
      for(auto it=other.begin(); it!=other.end(); ++it)
      {
        append(*it);
      }
    }
    return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
    if(this!=&other)
    {
      this->erase(cbegin(),cend());
      delete head;
      this->numberOfNodes=other.numberOfNodes;
      this->head=other.head;
      this->tail=other.tail;

      other.numberOfNodes=0;
      other.head=nullptr;
      other.tail=nullptr;
    }
    return *this;
  }

  bool isEmpty() const
  {
    if(numberOfNodes)
    {
      return false;
    }
    return true;
  }

  size_type getSize() const
  {
    return numberOfNodes;
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
    Node* newItem=new Node(item);
    newItem->nextPtr=insertPosition.getPtr();
    newItem->prevPtr=insertPosition.getPtr()->prevPtr;
    if(insertPosition.getPtr()->prevPtr!=nullptr)
    {
      insertPosition.getPtr()->prevPtr->nextPtr=newItem;
    }
    else
    {
      head=newItem;
    }
    insertPosition.getPtr()->prevPtr=newItem;
    numberOfNodes++;
    return;
  }

  Type popFirst()
  {
    if(numberOfNodes==0)
    {
      throw std::out_of_range("Empty collection");
    }
    Type temp=*(cbegin());
    erase(cbegin());
    return temp;
  }

  Type popLast()
  {
    if(numberOfNodes==0)
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
    if(possition==cbegin())
    {
      head=possition.getPtr()->nextPtr;
    }
    else
    {
      possition.getPtr()->prevPtr->nextPtr=possition.getPtr()->nextPtr;
    }
    possition.getPtr()->nextPtr->prevPtr=possition.getPtr()->prevPtr;

    delete possition.getPtr();
    numberOfNodes--;
    return;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    const_iterator it(firstIncluded);
    for(;it!=lastExcluded;it++)
    {
      erase(it);
    }
    return;
  }

  iterator begin()
  {
    return Iterator(head);
  }

  iterator end()
  {
    return Iterator(tail);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(head);
  }

  const_iterator cend() const
  {
    return ConstIterator(tail);
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
class LinkedList<Type>::ConstIterator
{
  Node* currentPtr;

public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

  explicit ConstIterator(Node *element)
  : currentPtr(element)
  {}

  reference operator*() const
  {
    if(currentPtr->nextPtr==nullptr)
    {
      throw std::out_of_range("Cannot dereference");
    }
    return(currentPtr->value);
  }

  ConstIterator& operator++()
  {
    if(currentPtr->nextPtr==nullptr)
    {
      throw std::out_of_range("Cannot dereference");
    }
    currentPtr=currentPtr->nextPtr;
    return *this;
  }

  ConstIterator operator++(int)
  {
    if(currentPtr->nextPtr==nullptr)
    {
      throw std::out_of_range("Cannot dereference");
    }
    ConstIterator temp =*this;
    currentPtr=currentPtr->nextPtr;
    return temp;
  }

  ConstIterator& operator--()
  {
    if(currentPtr->prevPtr==nullptr)
    {
      throw std::out_of_range("Cannot dereference");
    }
    currentPtr=currentPtr->prevPtr;
    return *this;
  }

  ConstIterator operator--(int)
  {
    if(currentPtr->prevPtr==nullptr)
    {
      throw std::out_of_range("Cannot dereference");
    }
    ConstIterator temp(*this);
    currentPtr=currentPtr->prevPtr;
    return temp;
  }

  ConstIterator operator+(difference_type d) const
  {
    ConstIterator temp(*this);
    for(difference_type i=0; i<d; i++)
    {
      temp++;
    }
    return temp;
  }

  ConstIterator operator-(difference_type d) const
  {
    ConstIterator temp(*this);
    for(difference_type i=0; i<d; i++)
    {
      temp--;
    }
    return temp;
  }

  bool operator==(const ConstIterator& other) const
  {
    return (currentPtr==other.currentPtr);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return (currentPtr!=other.currentPtr);
  }

  Node* getPtr() const
  {
    return currentPtr;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(Node *element)
  : ConstIterator(element)
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

#endif // AISDI_LINEAR_LINKEDLIST_H
