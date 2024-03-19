#include <gtest/gtest.h>

#include "Executor.h"

class InterpreterTest : public testing::Test {
protected:
	seal::Context context{};
	InterpreterTest() : parser(tokens), interpreter(&context) {}
	seal::impl::Lexer lexer;
	seal::impl::Parser parser;
	seal::impl::Interpreter interpreter;
	std::vector <seal::impl::Token> tokens;
	std::string code;
};

TEST_F(InterpreterTest, VariableValueCheck) {
	code = "fish is 77";
	lexer.ConvertStringToTokens(code, tokens);
	parser = {tokens};
	std::unique_ptr<seal::impl::ASTNode> prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 77); // normal assignment
	tokens.clear();
	code.clear();
	context.variables.clear();
	code = "fish is spit 1";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, -1); // negative number assignment
	tokens.clear();
	code.clear();
	context.variables.clear();
	code = "fish is (1)";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 1); // brackets expression assignment
	tokens.clear();
	code.clear();
	context.variables.clear();
	code = "oyster is 77 \n fish is oyster";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 77); // variable assignment
	tokens.clear();
	code.clear();
	context.variables.clear();
	code = "oyster is 77 \n fish is (oyster)";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 77); // in-brackets variable assignment
	tokens.clear();
	code.clear();
	context.variables.clear();
	code = "fish is 77 bite 10";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 7); // division expression assignment
	tokens.clear();
	code.clear();
	context.variables.clear();
	code = "fish is 77 \n fish is fish bite 10";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 7); // self division expression assignment
	tokens.clear();
	code.clear();
	context.variables.clear();
	code = "fish is oyster";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	EXPECT_THROW(prog->Accept(interpreter); , seal::impl::Exception); // unknown variable assignment
	tokens.clear();
	code.clear();
	context.variables.clear();
	code = "fish is 2147483650";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	EXPECT_THROW(prog = parser.ParseProgram(), seal::impl::Exception); // value larger than int 
	tokens.clear();
	code.clear();
	context.variables.clear();
	code = "fish is 0101";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 101); // excess zero number assignment
	tokens.clear();
	code.clear();
	context.variables.clear();
	context.variables.insert({ "fish", seal::Value{77}});
	code = "fish is fish spit 1";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 76); // assignment via context - int
	tokens.clear();
	code.clear();
	context.variables.clear();
	context.variables.insert({ "fish", seal::Value{'a'}});
	code = "fish is fish spit 1";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 96); // assignment via context - char -> questionable, but.... ok
	tokens.clear();
	code.clear();
	context.variables.clear();
	context.variables.insert({ "fish", seal::Value{'\n'} });
	code = "fish is fish spit 1";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 9); // assignment via context - char -> yeah, it's definitely interesting 
	tokens.clear();
	code.clear();
	context.variables.clear();
	context.variables.insert({ "fish", seal::Value{(2)}});
	code = "fish is fish spit 1";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 1); //  assignment via context - in-brackets number
	tokens.clear();
	code.clear();
	context.variables.clear();
	context.variables.insert({ "fish", seal::Value{1 + 1} });
	code = "fish is fish spit 1";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 1); //  assignment via context - expression
	tokens.clear();
	code.clear();
	context.variables.clear();
	int oyster = 5;
	context.variables.insert({ "fish", seal::Value{oyster + oyster} });
	code = "fish is fish spit 1";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, 9); //  assignment via context - expression with outer-code variables
	tokens.clear();
	code.clear();
	context.variables.clear();
	context.variables.insert({ "fish", seal::Value{} });
	code = "fish is fish spit 1";
	lexer.ConvertStringToTokens(code, tokens);
	parser = { tokens };
	prog = parser.ParseProgram();
	prog->Accept(interpreter);
	EXPECT_EQ(context.variables.at("fish").num_value, -1); //  assignment via context - empty value
}