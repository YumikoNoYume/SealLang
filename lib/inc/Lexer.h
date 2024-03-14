#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "ErrorManager.h"

namespace seal {
	namespace impl {
		
		enum class TokenType { snif, bark, bouncing, slap, sneeze, new_line, bracket_left, bracket_right, eat, spit, imagine, bite, is, tasty, small, big, nah, identificator, literal };

		struct Token {
			Token() = default;
			Token(TokenType tt, std::string l, std::size_t i);
			TokenType type;
			std::string lexeme;
			std::size_t line;
		};

		class Lexer {
		public:
			Lexer();
			void ConvertStringToTokens(const std::string& p, std::vector<Token>& t);
		private:
			void Push(const std::string& p, std::size_t l, std::vector<Token>& t, std::string& to_token);
			void CheckIfNewLine(const char& to_check, std::size_t& line, std::vector<Token>& vector, std::string& token);
			void CheckIfRBracket(const char& to_check, std::size_t line, std::vector<Token>& vector, std::string& token);
			void TurnToToken(std::string& to_token, const char ch, std::size_t& line, std::vector<Token>& t);
			std::unordered_map<std::string, std::function<TokenType()>> token_list;
			ErrorManager em;
		};

	}
}
