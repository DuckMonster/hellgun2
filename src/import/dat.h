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
	Array<Key_Value> children;

	Dat_Object() : Dat_Node(NODE_Object) {}
	~Dat_Object() 
	{
		for(const auto& child : children)
			delete child.value;

		children.reset();
	}
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
	~Dat_File() { free(); }
	void load_file(const String& path);
	void free();

	// Fetching
	Dat_Value* find_value(Dat_Object* obj, const char* path);
	void read_data(const char* path, void* data, const char* specifier);
	template<typename T>
	T read_simple(const char* path, const char* specifier)
	{
		T result;
		read_data(path, &result, specifier);

		return result;
	}

	bool contains_value(const char* path) { return find_value(root, path) != nullptr; }
	u32 read_u32(const char* path) { return read_simple<u32>(path, "%u"); }
	i32 read_i32(const char* path) { return read_simple<i32>(path, "%d"); }
	float read_f32(const char* path) { return read_simple<float>(path, "%f"); }

	String read_str(const char* path);
	TString read_str_temp(const char* path);

	Dat_Object* root = nullptr;
	char* buffer = nullptr;
};