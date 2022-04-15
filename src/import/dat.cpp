#include "dat.h"
#include "dat_parse.h"
#include "core/io.h"

void Dat_File::load_file(const char* path)
{
	u32 buffer_size;
	load_whole_file(path, &buffer, (int*)&buffer_size);

	Dat_Parse parser;
	root = parser.parse(buffer, buffer_size);

	if (parser.has_error)
		printf("Parsing dat file '%s' resulted in errors\n", path);
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

u32 Dat_File::read_u32(const char* path)
{
	u32 result = 0;

	Dat_Value* node = find_value(root, path);
	if (node == nullptr)
	{
		printf("Dat path '%s' was not valid\n", path);
		return result;
	}

	sscanf_s(node->token.ptr, "%u", &result);

	return result;
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