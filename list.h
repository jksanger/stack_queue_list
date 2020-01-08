#ifndef CS20A_LIST_H
#define CS20A_LIST_H

#include<iostream>
#include<assert.h>
// Linked List object that maintains both head and tail pointers
// and the size of the list.  Note that you have to keep the head,
// tail and size consistent with the intended state of the List 
// otherwise very bad things happen. 
template<typename Type>
class List {
public:

	List();

	List(const List<Type>& other);

	List<Type>& operator=(const List<Type>& other);

	~List();

	void	print() const;
	bool	empty() const;

	void	push_front(const Type &item);
	void	push_back(const Type &item);
	void	add(int index, const Type &item);

	// Note that the user must first ensure the list is not empty
	// prior to calling these functions.
	Type	front() const;
	Type	rear() const;
	Type	get_at(int index) const;

	int		size() const;
	int		find(const Type &item) const;

	bool	pop_front();
	bool	pop_rear();
	bool	pop_at(int index);

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif
	// Forward declare the nodes for our List.
	// Will be implemented along with list's
	// member functions
	class Node;

	// We'll have both head and tail points for 
	// Fast insertion/deletion from both ends.
	Node*	m_head;
	Node*	m_tail;

	// Keep track of size of list
	int		m_size;
};


// List Implementation
//
//  Since List is a template class (which is not an actual 
//  class yet, not until we actually instantiate the list) 
//  we need to keep the implementation together with 
//  the definition.  There are ways to simulate having 
//  separate "implementation/definition" with templates, 
//  but they aren't necessary and can be confusing.
//

// Node definition
//		Already implemented, nothing to do here but to use it.
template<typename Type>
class List<Type>::Node {
public:
	Node() :next(nullptr), prev(nullptr) {}
	Node(Type item, Node* p, Node* n) : item(item), next(n), prev(p) {}

	Type  item;
	Node* next;
	Node* prev;
};



// List default constructor
//		Set head and tail pointer to point to nothing, size is zero
//		Already implemented, nothing to do.
template<typename Type>
List<Type>::List() :m_head(nullptr), m_tail(nullptr), m_size(0) {
}


// Copy constructor  
//		This should create an entirely new linked list with the same 
//		number of Nodes and the Values stored these Nodes in the same 
//		order as seen the other list’s Nodes.  This should not result 
//		in any memory leaks or aliasing.  
template<typename Type>
List<Type>::List(const List<Type>& other) {
	size = other.size;
	Node *next = other.m_head;
	if (other.m_head != nullptr) {

		Node *n = other.m_head;
		m_head = new Node(n->item, nullptr, n->next);
		Node *pr = n;
		n = n->next;

		while (n->next != nullptr) {
			pr->next = new Node(pr->next->item, pr, pr->next->next);

			pr = pr->next;

			n = n->next;

		}
		m_tail = n;

	}
	else {
		m_head = nullptr;
		m_tail = nullptr;
	}
}


// Overloaded assignment operator 
//		Causes the already existing linked list to be identical to the 
//		other linked list without causing any memory leaks or aliasing.
template<typename Type>
List<Type>& List<Type>::operator=(const List<Type>& other) {
	if (&other != this) {
		size = other.size;
		m_head = other.m_head;
		m_tail = other.m_tail;
	}

	return *this;
}


// List destructor
//		Traverses the nodes and deletes them without causing memory leaks
template<typename Type>
List<Type>::~List() {
	while (m_head != nullptr)
	{
		Node* n = m_head->next;
		delete m_head;
		m_head = n;
	}
	m_size = 0;
	m_head = nullptr;
	m_tail = nullptr;
}


// List print
//		Traverses list and prints items in list to console
//		Assumes << is overloaded for object Type.
//		

template<typename Type>
void List<Type>::print() const {
	Node *p;
	p = m_head;
	while (p != nullptr)
	{
		std::cout << p->item << std::endl;
		p = p->next;
	}
}

// List empty
//		returns true if there are no items in the list
//		otherwise returns false.
template<typename Type>
bool List<Type>::empty() const {
	return (m_head == nullptr);

}


// List push_front
//		Adds a new item to the front of the list
//		updates head, tail, and size accordingly.
//		
//		Two cases to consider: If the list is empty 
//		and if the list has items already it.
//
template<typename Type>
void List<Type>::push_front(const Type &item) {
	Node *p;
	p = new Node;
	p->item = item;
	if (m_head == nullptr) {
		m_head = p;
		m_tail = p;
		p->next, p->prev = nullptr;
		m_size++;
	}
	else {
		p->next = m_head;
		p->prev = nullptr;
		m_head = p;
		p->next->prev = p;
		m_size++;
	}
}

// List push_back
//		Adds a new item to the rear of the list
//		updates head, tail, and size accordingly.
//
//		Two cases to consider: If the list is empty 
//		and if the list has items already it.
//
template<typename Type>
void List<Type>::push_back(const Type &item) {
	if (m_head == nullptr)
		push_front(item);
	else
	{
		Node *p = new Node;
		p->item = item;
		m_tail->next = p;
		p->next = nullptr;
		p->prev = m_tail;
		m_tail = p;
		m_size++;
	}
}

// List add
//		Adds a new item at the index indicated
//		updates head, tail, and size accordingly.
//
//		If the index less than or equal to zero add to the front.
//		If the index is greater than or equal to
//			the size of the list, add to the rear.
//		All other cases, add the item to the appropriate
//			location
//
template<typename Type>
void List<Type>::add(int index, const Type &item) {
	if (index <= 0)
		push_front(item);
	else if (index >= m_size)
		push_back(item);
	else {
		int counter = 0;
		Node* p = m_head;
		while (counter < index - 1)
		{
			p = p->next;
			counter++;
		}
		Node* n = new Node;
		n->item = item;
		n->next = p->next;
		n->prev = p;
		p->next = n;
		n->next->prev = n;
		m_size++;
	}

}

// 8. List front
//		returns the first item in the list.
//
template<typename Type>
Type List<Type>::front() const {
	// Force check that the head is pointing to a Node
	// Typical solution for deployement code is to throw
	// exceptions, but since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(m_head != nullptr);

	
	return m_head->item;
}

// List rear
//		returns the last item in the list
//
template<typename Type>
Type List<Type>::rear() const {

	// Force check that the tail is pointing to a Node
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(m_tail != nullptr);

	return m_tail->item;
	
}

// List get_at
//		returns the item at index
//
template<typename Type>
Type List<Type>::get_at(int index) const {

	// Force index to be correct before getting the Item
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(index >= 0 && index < m_size);

	int counter = 0;
	Node* p = m_head;
	while (counter < index)
	{
		p = p->next;
		counter++;
	}
	return p->item;
}

// List size
//		return the size of the list
template<typename Type>
int List<Type>::size() const {
	
	return m_size;
}

// List find
//		Traverses the list to see if the item is in the list
//		if it is return the index of that item, otherwise
//		return -1. Assumes the == operator is overloaded for
//		object Type.
template<typename Type>
int List<Type>::find(const Type &item) const {

	Node* p = m_head;
	int counter = 0;
	while (p != nullptr)
	{
		if (p->item == item)
			return counter;
		p = p->next;
		counter++;
	}
	return -1;
}


// List pop_front
//		Removes the first item in the list returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly.
//
//		Must consider at least two cases where the list: is empty, 
//		has one or more items.
//		
template<typename Type>
bool List<Type>::pop_front() {

	if (m_head == nullptr) {
		return false;
	}
	Node *p = m_head;
	if (p->next == nullptr) {
		m_head = nullptr;
		m_tail = nullptr;
		delete p;
		m_size--;
		return true;
	}
	else {
		m_head = p->next;
		m_head->prev = nullptr;
		delete p;
		m_size--;
		return true;
	}
}

// List pop_rear
//		Removes the last item in the list returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly.
//
//		Must consider at least three cases where the list: is empty, 
//		has one item, has two or more items.
//	
template<typename Type>
bool List<Type>::pop_rear() {
	if (m_head == nullptr)
		return false;

	Node *p = m_head;

	if (p->next == nullptr)
	{
		delete p;
		m_head = m_tail = nullptr;
		m_size--;
		return true;
	}
	else
	{
		while (p->next->next != nullptr)
			p = p->next;
		delete p->next;
		p->next = nullptr;
		m_tail = p;
		m_size--;
		return true;
	}
}


// List pop_at
//		Removes the item at the index returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly. If the index is
//		out of bounds return false.
//
template<typename Type>
bool List<Type>::pop_at(int index) {

	if (index < 0 || index >= m_size)
		return false;
	if (index == 0)
		pop_front();
	else if (index == m_size - 1)
		pop_rear();
	else
	{
		int counter = 0;
		Node* p = m_head;
		while (counter < index - 1)
		{
			p = p->next;
			counter++;
		}
		Node* temp = p->next;
		p->next = temp->next;
		delete p;
	}
	m_size--;
	return true;

}

#endif