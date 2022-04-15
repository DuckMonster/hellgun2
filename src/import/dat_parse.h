#pragma once
#include "dat.h"

class Dat_Object;
struct Dat_Parse
{
	Dat_Object* parse(char* buffer, u32 len);

	bool read_token(Dat_Token* token);
	bool peek_token(Dat_Token* token);
	bool token_expect(Dat_Token* token, Dat_Token_Type type);

	Dat_Node* read_node();
	Dat_Object* read_object();

	void skip_whitespace();
	void skip_newlines();
	bool eof();

	int col(const char* c);
	int row(const char* c);

	void throw_unexpected(const char* where, const String& message);

	bool has_error = false;

	char* buffer;
	char* ptr = nullptr;
	u32 buffer_size = 0;
};