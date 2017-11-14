//Multiset.cpp
#include "Multiset.h"
#include <iostream>
#include <cassert>

Multiset::Multiset()
{
	m_head = new node;
	m_head->m_next = m_head; //dummy node
	m_head->m_prev = m_head;
	m_unique = 0;
	m_total = 0;
}
Multiset::~Multiset()
{
	node* traverser = m_head->m_next; //starts at node after m_head
	node* p;
	while (traverser != m_head)
	{
		p = traverser->m_next;
		delete traverser;
		traverser = p; //traverser advances
	}
	delete m_head; //deletes dummy node last
}

Multiset::Multiset(const Multiset& other)
{
	m_unique = other.m_unique; //copy data
	m_total = other.m_total;
	m_head = new node;   //make a new dummy node
	m_head->m_next = m_head;
	m_head->m_prev= m_head;

	node* traverser = other.m_head->m_next;

	while(traverser != other.m_head)
	{
		node* newguy = new node; //make a new node and copy the values from the other node
		newguy->data = traverser->data;
		newguy->m_count = traverser->m_count;

		node* temp = m_head->m_prev; //add the copied node to the end of the new linked list
		newguy->m_next =m_head;
		newguy->m_prev = temp;
		newguy->m_prev->m_next = newguy;
		m_head->m_prev = newguy;
		traverser = traverser->m_next;
	}
}

Multiset& Multiset::operator=(const Multiset& other)
{
	if(this != &other)
	{
		Multiset temp(other); //make a newly linked list and call swap to swap pointers and data
		swap(temp); 
	}
	return *this;
}

bool Multiset::empty() const
{
	if(m_head->m_next == m_head)
		return true;
	else 
		return false;
}
int Multiset::size() const
{
	//returns total items
	return m_total;
}
int Multiset::uniqueSize() const
{
	//returns distinct items
	return m_unique;
}
bool Multiset::insert(const ItemType& value)
{
	node* traverser = m_head->m_next;
	while(traverser != m_head)
	{
		if(traverser->data == value) //if the value already exists in the list
		{
			traverser->m_count++;
			m_total++;
			return true;
		}
		traverser = traverser->m_next;
	}
	//if the value did not previously exist in the list
	node* newguy = new node;
	newguy->data = value;
	node* temp = m_head->m_prev;
	newguy->m_prev = temp;
	newguy->m_next = m_head;
	m_head->m_prev = newguy;
	newguy->m_prev->m_next = newguy;
	newguy->m_count = 1;
	m_total++;
	m_unique++;
	return true;
}
int Multiset::erase(const ItemType& value)
{
	node* traverser = m_head->m_next;
	while (traverser != m_head)
	{
		if(traverser->data == value) //if we find the value
		{
			traverser->m_count--;
			m_total--;
			if(traverser->m_count == 0) //if there are no more instances after removing one
			{
				m_unique--;
				traverser->m_prev->m_next = traverser->m_next;//unlink the node
				traverser->m_next->m_prev = traverser->m_prev;
				delete traverser; //delete the node
			}
			return 1;
		}
		traverser = traverser->m_next;
	}
	return 0; //if we don't find the value
}
int Multiset::eraseAll(const ItemType& value)
{
	node* traverser = m_head->m_next;
	int returnvalue=0; //default return value
	while (traverser != m_head)
	{
		if(traverser->data == value)
		{
			m_unique--;
			returnvalue = traverser->m_count;
			m_total -= traverser->m_count; //reduce total number of elements
			traverser->m_prev->m_next = traverser->m_next;
			traverser->m_next->m_prev = traverser->m_prev;
			delete traverser; //delete the node
			return returnvalue;
		}
		traverser = traverser->m_next;
	}
	return returnvalue;
}
bool Multiset::contains(const ItemType& value) const
{
	// Return true if the value is in the multiset, otherwise false.
	node* traverser = m_head->m_next;

	while (traverser != m_head)
	{
		if(traverser->data == value)
			return true;
		traverser = traverser->m_next;
	}
	return false;
}
int Multiset::count(const ItemType& value) const
{
	// Return the number of instances of value in the multiset.
	node* traverser = m_head->m_next;

	while(traverser != m_head)
	{
		if(traverser->data == value)
			return traverser->m_count;
		traverser = traverser->m_next;
	}
	return 0;
}
int Multiset::get(int i, ItemType& value) const
{
	// If 0 <= i < uniqueSize(), copy into value an item in the
          // multiset and return the number of instances of that item in
          // the multiset.  Otherwise, leave value unchanged and return 0.
	if(i < 0 || i >=uniqueSize())
		return 0;
	node* traverser = m_head->m_next;
	int slot = 0; //starts at the node after the node m_head points to
	while (traverser != m_head) //will break if the value is not found
	{
		if(slot == i)
		{
			value = traverser->data;
			return traverser->m_count;
		}
		slot++;
		traverser = traverser->m_next;
	}
	return 0;
}
void Multiset::swap(Multiset& other)
{
	 // Exchange the contents of this multiset with the other one.
	if(this != &other){
	node* temp = other.m_head;
	other.m_head = m_head;
	m_head = temp;

	int copytotal = m_total; //also swap the other member elements of the multiset
	m_total = other.m_total;
	other.m_total = copytotal;

	int copyunique = m_unique;
	m_unique = other.m_unique;
	other.m_unique = copyunique;
	}
}

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	//assign ms1 into result, insert() the values of ms2 into result

	result = ms1;
	ItemType f;
	for(int k = 0; k< ms2.uniqueSize(); k++)
	{
		int n = ms2.get(k,f);
		for( int w = 0; w<n; w++)
		{ 
			result.insert(f);
		}
	}
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	//assign ms1 into result, erase() the values from ms2 from result
	result = ms1;
	ItemType f;
	for(int k=0; k<ms2.uniqueSize(); k++)
	{
		int n = ms2.get(k,f);
		for(int w = 0; w<n; w++)
		{
			result.erase(f);
		}
	}
}