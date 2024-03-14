#include "Lexer.h"

namespace seal {
	namespace impl {
		
		Token::Token(TokenType tt, std::string l, std::size_t i) : type(tt), line(i), lexeme(l) {}

		Lexer::Lexer() : em() {
			token_list = {
			{"bark",[]() -> TokenType {return TokenType::bark; }},
			{"big", []() -> TokenType {return TokenType::big; }},
			{"bite", []() -> TokenType {return TokenType::bite; }},
			{"bouncing", []() -> TokenType {return TokenType::bouncing; }},
			{"(", []() -> TokenType {return TokenType::bracket_left; }},
			{")", []() -> TokenType {return TokenType::bracket_right; }},
			{"eat", []() -> TokenType {return TokenType::eat; }},
			{"imagine", []() -> TokenType {return TokenType::imagine; }},
			{"is", []() -> TokenType {return TokenType::is; }},
			{"nah", []() -> TokenType {return TokenType::nah; }},
			{"\n", []() -> TokenType {return TokenType::new_line; }},
			{"slap", []() -> TokenType {return TokenType::slap; }},
			{"small", []() -> TokenType {return TokenType::small; }},
			{"sneeze", []() -> TokenType {return TokenType::sneeze; }},
			{"snif", []() -> TokenType {return TokenType::snif; }},
			{"spit", []() -> TokenType {return TokenType::spit; }},
			{"tasty", []() -> TokenType {return TokenType::tasty; }},
			{"lit", []() -> TokenType {return TokenType::literal; }},
			{"var", []() -> TokenType {return TokenType::identificator; }}
			};
		}

		void Lexer::Push(const std::string& to_add, std::size_t line, std::vector<Token>& vector, std::string& token)  {
			Token to_push;
			if (to_add == "lit" || to_add == "var") {
				to_push = { token_list[to_add](), token, line };
			}
			else {
				to_push = { token_list[to_add](), "", line };
			}
			vector.push_back(to_push);
			token.clear();
		}

		void Lexer::CheckIfNewLine(const char& to_check, std::size_t& line, std::vector<Token>& vector, std::string& token) {
			if (to_check == '\n') {
				Push("\n", line, vector, token);
				line++;
			}
		}

		void Lexer::CheckIfRBracket(const char& to_check, std::size_t line, std::vector<Token>& vector, std::string& token)  {
			if (to_check == ')') {
				Push(")", line, vector, token);
			}
		}

		void Lexer::TurnToToken(std::string& to_token, const char ch, std::size_t& line, std::vector<Token>& t) {
			if (ch == '(') {
				if (to_token.size() > 0) {
					Push(to_token, line, t, to_token);
				}
				Push("(", line, t, to_token);
			}
			else if (token_list.find(to_token) != token_list.end()) {
				Push(to_token, line, t, to_token);
				if (t.back().type == TokenType::sneeze) {
					CheckIfNewLine(ch, line, t, to_token);
				}
				CheckIfRBracket(ch, line, t, to_token);
			}
			else {
				try {
					const int i = std::stoi(to_token);
					Push("lit", line, t, to_token);
					CheckIfNewLine(ch, line, t, to_token);
					CheckIfRBracket(ch, line, t, to_token);
				}
				catch (std::invalid_argument const& ex) {
					if (to_token.empty()) { 
						CheckIfNewLine(ch, line, t, to_token);
						CheckIfRBracket(ch, line, t, to_token);
						return; }
					if (std::isalpha(*(to_token.begin()))) {
						Push("var", line, t, to_token);
						CheckIfNewLine(ch, line, t, to_token);
						CheckIfRBracket(ch, line, t, to_token);
						return;
					}
					em.CollectError(Error(line, 1));
					to_token.clear();
				}
			}
		}
		
		void Lexer::ConvertStringToTokens(const std::string& p, std::vector<Token>& t) {
			std::string to_token;
			std::size_t line = 1;
			for (auto ch : p) {
				if (!std::isdigit(ch) && !std::isalpha(ch) && ch != ' ' && ch != '\n' && ch != '(' && ch != ')' && ch != '\t') {
					em.CollectError(Error(line, 1));
				}
				if (ch != ' ' && ch != '\n' && ch != '(' && ch != ')' && ch != '\t') {
					to_token.push_back(ch);
				}
				else {
					TurnToToken(to_token, ch, line, t);
				}
			}
			if (to_token.size() > 0) {
				TurnToToken(to_token, ' ', line, t);
			}
			if (t.size() == 0 || t.back().type != TokenType::new_line) {
				CheckIfNewLine('\n', line, t, to_token);
			}
			if (em.GetErrorListSize() > 0) {
				throw (Exception(std::move(em.GetErrorMessages())));
			}
		}
	}
}
