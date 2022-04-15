#include "dat_parse.h"
#include "core/io.h"

inline bool alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
inline bool digit(char c) { return c >= '0' && c <= '9'; } 
inline bool newline(char c) { return c == '\n' || c == '\r'; }
inline bool whitespace(char c) { return c == ' ' || c == '\t'; }

const char* dat_token_type_str(Dat_Token_Type type)
{
	switch(type)
	{
		case TOKEN_Invalid: return "INVALID";
		case TOKEN_Eof: return "end-of-file";
		case TOKEN_Keyword: return "keyword";
		case TOKEN_Number: return "number";
		case TOKEN_String: return "string";
		case TOKEN_NewLine: return "newline";
		default:
		{
			static char chr_string[2];
			chr_string[0] = type;
			chr_string[1] = 0;

			return chr_string;
		}
	}
}

Dat_Object* Dat_Parse::parse(char* buffer, u32 len)
{
	this->buffer = buffer;
	this->buffer_size = len;
	this->ptr = buffer;
	this->has_error = false;

	Dat_Object* obj = read_object();

	// Clean up
	this->buffer = nullptr;
	this->buffer_size = 0;
	this->ptr = nullptr;

	return obj;
}

bool Dat_Parse::read_token(Dat_Token* token)
{
	static Dat_Token temp_token;
	if (token == nullptr)
		token = &temp_token;

	skip_whitespace();

	token->ptr = ptr;
	token->length = 1;

	if (eof())
	{
		token->type = TOKEN_Eof;
		return false;
	}

	if (newline(*ptr))
	{
		token->type = TOKEN_NewLine;
		while(newline(*++ptr))
			token->length++;

		return true;
	}

	if (alpha(*ptr))
	{
		token->type = TOKEN_Keyword;
		token->length = 0;
		while(alpha(*ptr) || digit(*ptr))
		{
			token->length++;
			ptr++;
		}

		return true;
	}

	if (digit(*ptr))
	{
		token->type = TOKEN_Number;
		while(digit(*++ptr))
			token->length++;

		return true;
	}

	if (*ptr == '"')
	{
		ptr++;

		token->type = TOKEN_String;
		token->length = 0;
		token->ptr = ptr;

		while(*ptr++ != '"')
		{
			token->length++;
			if (newline(*ptr))
			{
				throw_unexpected(ptr, "Unexpected newline, expected '\"'\n");
				return false;
			}
		}

		return true;
	}

	// Special characters
	if (*ptr == ':' || *ptr == ';' || *ptr == '{' || *ptr == '}')
	{
		token->type = (Dat_Token_Type)*ptr;
		token->length = 1;
		ptr++;
		return true;
	}

	throw_unexpected(ptr, String::printf("Unexpected token '%c'", *ptr));
	return false;
}
bool Dat_Parse::peek_token(Dat_Token* token)
{
	char* prev_ptr = ptr;
	bool result = read_token(token);

	ptr = prev_ptr;
	return result;
}

bool Dat_Parse::token_expect(Dat_Token* token, Dat_Token_Type type)
{
	static Dat_Token temp_token;
	if (token == nullptr)
		token = &temp_token;

	if (!read_token(token))
	{
		throw_unexpected(token->ptr, String::printf("Failed to read token, expected '%s'", dat_token_type_str(type)));
		return false;
	}

	if (token->type != type)
	{
		throw_unexpected(token->ptr, String::printf("Unexpected token '%s', expected '%s'", dat_token_type_str(token->type), dat_token_type_str(type)));
		return false;
	}

	return true;
}

Dat_Node* Dat_Parse::read_node()
{
	Dat_Token token;
	if (!read_token(&token))
		return nullptr;

	switch(token.type)
	{
		case TOKEN_Number:
		case TOKEN_String:
			return new Dat_Value(token);

		case '{':
		{
			Dat_Object* obj = read_object();

			skip_newlines();
			token_expect(nullptr, (Dat_Token_Type)'}');

			return obj;
		}

		default:
			throw_unexpected(token.ptr, String::printf("Unexpected token '%.*s'", token.length, token.ptr));
			return nullptr;
	}

	return nullptr;
}
Dat_Object* Dat_Parse::read_object()
{
	Dat_Object* obj = new Dat_Object();

	// Read key value pairs
	while(true)
	{
		skip_newlines();

		Dat_Token key_token;
		peek_token(&key_token);

		// Well, this isn't a key, so I suppose we're done here
		if (key_token.type != TOKEN_Keyword)
			return obj;

		// Read it after peeking
		read_token(nullptr);

		// If we read a key, we sure as hell expect a ':'
		if (!token_expect(nullptr, (Dat_Token_Type)':'))
			return obj;

		skip_newlines();

		// Read the value
		Dat_Node* value = read_node();
		if (value == nullptr)
			return obj;

		obj->add_child(key_token, value);

		// Check for ; or newline
		Dat_Token separator;
		if (peek_token(&separator))
		{
			if (separator.type == (Dat_Token_Type)';')
				read_token(nullptr);
			else if (separator.type == TOKEN_NewLine)
				read_token(nullptr);
			else
			{
				throw_unexpected(separator.ptr, String::printf("Unexpected token '%.*s', expected an object separator (newline or ';')", separator.length, separator.ptr));
				return obj;
			}
		}
	}

	return nullptr;
}

void Dat_Parse::skip_whitespace()
{
	while(!eof() && whitespace(*ptr))
		ptr++;
}
void Dat_Parse::skip_newlines()
{
	while(!eof() && (newline(*ptr) || whitespace(*ptr)))
		ptr++;
}

bool Dat_Parse::eof()
{
	if (buffer == nullptr)
		return true;

	return (ptr - buffer) >= buffer_size;
}

int Dat_Parse::col(const char* c)
{
	int col = 0;

	while(c >= buffer)
	{
		if (*c == '\n')
			return col;

		col++;
		c--;
	}

	return col;
}

int Dat_Parse::row(const char* c)
{
	int row = 1;

	while(c > buffer)
	{
		if (*c == '\n')
			row++;

		c--;
	}

	return row;
}

void Dat_Parse::throw_unexpected(const char* where, const String& message)
{
	printf("Dat error (%d:%d) - %s\n", row(where), col(where), message.data());
	has_error = true;
}