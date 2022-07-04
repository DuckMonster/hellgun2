#include "dat.h"
#include "dat_parse.h"
#include "core/io.h"

void Dat_File::free()
{
	if (root)
	{
		delete root;
		root = nullptr;
	}

	if (buffer)
	{
		::free(buffer);
		buffer = nullptr;
	}
}

void Dat_File::load_file(const String& path)
{
	free();

	u32 buffer_size;
	load_whole_file(path.data(), &buffer, (int*)&buffer_size);

	Dat_Parse parser;
	root = parser.parse(buffer, buffer_size);

	if (parser.has_error)
		printf("Parsing dat file '%s' resulted in errors\n", path.data());
}

Dat_Value* Dat_File::find_value(Dat_Object* obj, const char* path)
{
	// Find keyword
	Dat_Node_Type expect_type = NODE_Value;

	const char* key_end = path;
	while(*key_end)
	{
		if (*key_end == '.')
		{
			expect_type = NODE_Object;
			break;
		}

		key_end++;
	}

	u32 len = key_end - path;
	Dat_Node* node = obj->find_child(path, len);
	if (node == nullptr)
		return nullptr;

	if (node->type != expect_type)
		return nullptr;

	// This isn't a value, we shall continue searching
	if (expect_type == NODE_Object)
		return find_value((Dat_Object*)node, key_end + 1);

	return (Dat_Value*)node;
}

void Dat_File::read_data(const char* path, void* data, const char* specifier)
{
	Dat_Value* value = find_value(root, path);
	if (value == nullptr)
	{
		printf("Dat path '%s' was invalid\n", path);
		return;
	}

	sscanf_s(value->token.ptr, specifier, data);
}

String Dat_File::read_str(const char* path)
{
	String result;

	Dat_Value* node = find_value(root, path);
	if (node == nullptr)
	{
		printf("Dat path '%s' was not valid\n", path);
		return result;
	}

	u32 len = node->token.length;

	result.resize(len);
	memcpy(result.data(), node->token.ptr, len);
	result[len] = 0;

	return result;
}

TString Dat_File::read_str_temp(const char* path)
{
	TString result;

	Dat_Value* node = find_value(root, path);
	if (node == nullptr)
	{
		printf("Dat path '%s' was not valid\n", path);
		return result;
	}

	u32 len = node->token.length;

	result.resize(len);
	memcpy(result.data(), node->token.ptr, len);
	result[len] = 0;

	return result;
}