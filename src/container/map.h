#pragma once
#include "math/hash.h"

template<typename TKey, typename TValue>
struct Map
{
private:
	struct Node
	{
		u64 hash;

		TKey key;
		TValue value;

		Node* parent = nullptr;
		Node* left = nullptr;
		Node* right = nullptr;
	};

public:
	Map()
	{
		for(Node*& root : buckets)
			root = nullptr;
	}
	~Map()
	{
		for(Node*& bucket : buckets)
		{
			if (bucket)
			{
				free_recursive(bucket);
				bucket = nullptr;
			}
		}
	}

	void add(const TKey& key, const TValue& value)
	{
		u64 hash = Hash::fnv64<TKey>(key);
		Node** bucket = &buckets[hash & 0xF];

		insert_node(bucket, hash, key, value);
	}

	template<typename TOtherKey>
	bool contains(const TOtherKey& key)
	{
		return find_node(Hash::fnv64<TOtherKey>(key)) != nullptr;
	}

	template<typename TOtherKey>
	TValue& operator[](const TOtherKey& key)
	{
		Node* node = find_node(Hash::fnv64<TOtherKey>(key));
		if (node == nullptr)
			fatal("Tried to fetch key from map that didn't exist");

#if DEBUG
		if (node->key != key)
			fatal("Hash match in map, but the value of the keys didn't match :(\nfnv64 not good enough?");
#endif

		return node->value;
	}
	template<typename TOtherKey>
	const TValue& operator[](const TOtherKey& key) const
	{
		Node* node = find_node(Hash::fnv64<TOtherKey>(key));
		if (node == nullptr)
			fatal("Tried to fetch key from map that didn't exist");

#if DEBUG
		if (node->key != key)
			fatal("Hash match in map, but the value of the keys didn't match :(\nfnv64 not good enough?");
#endif

		return node->value;
	}

private:
	void insert_node(Node** node, u64 hash, const TKey& key, const TValue& value)
	{
		while(*node != nullptr)
		{
			if ((*node)->hash == hash)
				fatal("Double insert of key in map");

			// Left or right?
			if (hash < (*node)->hash)
				node = &((*node)->left);
			else
				node = &((*node)->right);
		}

		*node = new Node();
		(*node)->hash = hash;
		(*node)->key = key;
		(*node)->value = value;
	}

	void free_recursive(Node* node)
	{
		if (node->left)
			free_recursive(node->left);
		if (node->right)
			free_recursive(node->right);

		delete node;
	}

	Node* find_node(u64 hash)
	{
		Node* node = buckets[hash & 0xF];
		while(node)
		{
			if (node->hash == hash)
				break;

			if (hash < node->hash)
				node = node->left;
			else
				node = node->right;
		}

		return node;
	}

	Node* buckets[16] = { nullptr };
};