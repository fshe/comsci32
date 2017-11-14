//Multiset.h

#ifndef MULTISET_INCLUDED
#define MULTISET_INCLUDED

typedef unsigned long ItemType;

class Multiset
{
  public:
    Multiset();
	~Multiset();
    Multiset(const Multiset& other);
	Multiset& operator=(const Multiset& other);
	bool empty() const;
    int size() const;
    int uniqueSize() const;
    bool insert(const ItemType& value);
    int erase(const ItemType& value);
    int eraseAll(const ItemType& value);
    bool contains(const ItemType& value) const;
    int count(const ItemType& value) const;
    int get(int i, ItemType& value) const;
    void swap(Multiset& other);
private:
	struct node
{
	ItemType data;
	int m_count;
	node* m_next;
	node* m_prev;
};
	node *m_head;
	int m_unique;
	int m_total;
};

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result);
void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result);

#endif //MULTISET_INCLUDED