#pragma once
#include "container/array.h"

enum Dat_Token_Type
{
	TOKEN_Invalid,
	TOKEN_Eof,
	TOKEN_Keyword,
	TOKEN_Number,
	TOKEN_String,
	TOKEN_NewLine,
};
const char* dat_token_type_str(Dat_Token_Type type);

enum Dat_Node_Type
{
	NODE_Invalid,
	NODE_Value,
	NODE_Object
};

struct Dat_Token
{
	Dat_Token_Type type;

	const char* ptr;
	u32 length;
};

class Dat_Node
{
public:
	Dat_Node_Type type;
	Dat_Node(Dat_Node_Type type) : type(type) {}
};

class Dat_Object : public Dat_Node
{
public:
	struct Key_Value
	{
		Dat_Token key;
		Dat_Node* value;
	};

	Dat_Object() : Dat_Node(NODE_Object) {}

	Array<Key_Value> children;
	void add_child(const Dat_Token& key, Dat_Node* value)
	{
		Key_Value child;
		child.key = key;
		child.value = value;

		children.add(child);
	}

	Dat_Node* find_child(const char* path, u32 len)
	{
		for(Key_Value& child : children)
		{
			if (child.key.length != len)
				continue;

			if (strncmp(child.key.ptr, path, len) == 0)
				return child.value;
		}

		return nullptr;
	}
};

class Dat_Value : public Dat_Node
{
public:
	Dat_Token token;
	Dat_Value(const Dat_Token& token) : Dat_Node(NODE_Value), token(token) {}
};

struct Dat_File
{
	void load_file(const char* path);

	// Fetching
	Dat_Value* find_value(Dat_Object* obj, const char* path);
	u32 read_u32(const char* path);
	String read_str(const char* path);

	Dat_Object* root = nullptr;
	char* buffer = nullptr;
};