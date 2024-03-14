#include <gtest/gtest.h>

#include "Executor.h"

class LexerTest : public testing::Test {
protected:
	seal::impl::Lexer lexer;
	std::vector <seal::impl::Token> tokens;
	std::string prog;
};

TEST_F(LexerTest, ZeroInput) {
	lexer.ConvertStringToTokens(prog, tokens);
	EXPECT_EQ(tokens.size(), 1);
}

TEST_F(LexerTest, TabularAndMultipleSpaceInput) {
	prog = "\tseal     fish";
	lexer.ConvertStringToTokens(prog, tokens);
	EXPECT_EQ(tokens.size(), 3);
}

TEST_F(LexerTest, MultipleBracketsInput) {
	prog = "(((seal eat fish)))";
	lexer.ConvertStringToTokens(prog, tokens);
	EXPECT_EQ(tokens.size(), 10);
}

TEST_F(LexerTest, NewLineInput) {
	prog = "\n\n\nseal\n";
	lexer.ConvertStringToTokens(prog, tokens);
	EXPECT_EQ(tokens.size(), 5);
}

TEST_F(LexerTest, KeyWordsInput) {
	prog = "bark bark0 bark() barkbark 0";
	lexer.ConvertStringToTokens(prog, tokens);
	EXPECT_EQ(tokens.size(), 8);
	EXPECT_EQ(tokens[0].type, seal::impl::TokenType::bark);
	EXPECT_EQ(tokens[1].type, seal::impl::TokenType::identificator);
	EXPECT_EQ(tokens[2].type, seal::impl::TokenType::bark);
	EXPECT_EQ(tokens[5].type, seal::impl::TokenType::identificator);
	EXPECT_EQ(tokens[6].type, seal::impl::TokenType::literal);
}

TEST_F(LexerTest, VariableError1Input) {
	prog = "_seal";
	EXPECT_THROW(lexer.ConvertStringToTokens(prog, tokens), seal::impl::Exception);
}

TEST_F(LexerTest, VariableError2Input) {
	prog = "se_al";
	EXPECT_THROW(lexer.ConvertStringToTokens(prog, tokens), seal::impl::Exception);
}

TEST_F(LexerTest, ErrorInput) {
	prog = "+";
	EXPECT_THROW(lexer.ConvertStringToTokens(prog, tokens), seal::impl::Exception);
}

TEST_F(LexerTest, CodeInput) {
	prog = "fish1 is 10 \n fish2 is 6 \n party is 10 \n bouncing (fish1 eat fish2) bite party nah small 2 \n fish2 is fish2 eat 2 \n slap fish2 tasty fish1 \n fish2 is fish2 imagine (3 spit 1) \n sneeze \n sneeze \n bark fish2 big fish1";
	lexer.ConvertStringToTokens(prog, tokens);
	EXPECT_EQ(tokens.size(), 54);
	EXPECT_EQ(tokens[52].line, 10);
}


