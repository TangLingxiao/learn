#ifndef SKIP_LIST
#define SKIP_LIST
#include <random>
#include <iostream>
#define MAX_LEVLE (16)
#define RANDOM_P (0.25)

struct SkipListNode {
	int data = 0;
	SkipListNode* backward = nullptr;
	struct SkipListLevel {
		struct SkipListNode *forward = nullptr;
	}level[];
};

class SkipList
{
public:
	~SkipList() {
		while(tail){
			SkipListNode* node = tail;
			tail = tail->backward;
			delete[](char*)node;
		}
	}

	SkipList *CreateList() {
		head = CreateNode(MAX_LEVLE, 0);
		level = 1;
		return this;
	}

	int RandomLevel() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::binomial_distribution<> d(MAX_LEVLE, RANDOM_P);
		return d(gen);
	}


	void InsertNode(int data) {
		SkipListNode* update[MAX_LEVLE];
		SkipListNode* node = head;
		for (int i = level - 1; i >= 0; --i) {
			while (node->level[i].forward && node->level[i].forward->data <= data) {
				node = node->level[i].forward;
			}
			update[i] = node;
		}

		int randomlevel = RandomLevel();

		if (randomlevel > level) {
			for (int i = level; i < randomlevel; ++i) {
				update[i] = head;
			}
			level = randomlevel;
		}

		SkipListNode* newnode = CreateNode(randomlevel, data);
		for (int i = 0; i < level; ++i) {
			newnode->level[i].forward = update[i]->level[i].forward;
			update[i]->level[i].forward = newnode;
		}

		newnode->backward = update[0] == head ? nullptr : update[0];

		if (newnode->level[0].forward) {
			newnode->level[0].forward->backward = newnode;
		}
		else {
			tail = newnode->level[0].forward;
		}
	}

	void DeleteNode(int data) {
		SkipListNode* update[MAX_LEVLE];
		SkipListNode* node = head;
		for (int i = level - 1; i >= 0; --i) {
			while (node->level[i].forward && node->level[i].forward->data < data) {
				node = node->level[i].forward;
			}
			update[i] = node;
		}
		node = node->level[0].forward;
		if (node && node->data == data) {
			for (int i = 0; i < level; ++i) {
				if (update[i]->level[i].forward == node) {
					update[i]->level[i].forward = node->level[i].forward;
				}
			}
			if (node->level[0].forward) {
				node->level[0].forward->backward = node->backward;
			}
			else {
				tail = node->backward;
			}
			while (level > 1 && head->level[level - 1].forward == nullptr) {
				level--;
			}
			delete [](char*)node;
		}
	}

	SkipListNode* FindNode(int data) {
		SkipListNode* node = head;
		
		for (int i = level - 1; i >= 0; --i) {
			while (node->level[i].forward&&node->level[i].forward->data <= data) {
				node = node->level[i].forward;
			}
		}
		if (node->data == data) {
			return node;
		}
		return nullptr;
	}
protected:
	SkipListNode *CreateNode(int level, int data) {
		SkipListNode* node = (SkipListNode*)new char[sizeof(SkipListNode) + level * sizeof(SkipListNode::SkipListLevel)];
		node->data = data;
		node->backward = nullptr;
		for (int i = 0; i < level; ++i)
		{
			node->level[i].forward = nullptr;
		}
		return node;
	}

private:
	SkipListNode *head = nullptr, *tail = nullptr;
	int level = 0;
	
};
#endif
