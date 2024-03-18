#include <gtest/gtest.h>

#include "Executor.h"

class ParserTest : public testing::Test {
protected:
	ParserTest() : parser(tokens) {}
	seal::impl::Parser parser;
	std::vector <seal::impl::Token> tokens;
	std::unique_ptr<seal::impl::ASTNode> parser_tree;
	bool check_result;
};

bool AreEqueal(seal::impl::ASTNode* ref_tree, seal::impl::ASTNode* parser_tree) {
	if (typeid(*ref_tree).name() != typeid(*parser_tree).name()) {
		return false;
	}
	if (typeid(*ref_tree) == typeid(seal::impl::StatementList)) {
		seal::impl::StatementList* ref = static_cast<seal::impl::StatementList*>(ref_tree);
		seal::impl::StatementList* parsed = static_cast<seal::impl::StatementList*>(parser_tree);
		for (std::size_t index = 0; index < ref->node_list.size(); index++) {
			if (!AreEqueal(ref->node_list[index].get(), parsed->node_list[index].get())) {
				return false;
			}
		}
		return true;
	} else if (typeid(*ref_tree) == typeid(seal::impl::Statement)) {
		seal::impl::Statement* ref = static_cast<seal::impl::Statement*>(ref_tree);
		seal::impl::Statement* parsed = static_cast<seal::impl::Statement*>(parser_tree);
		return AreEqueal(ref->node.get(), parsed->node.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::AssignmentStatement)) {
		seal::impl::AssignmentStatement* ref = static_cast<seal::impl::AssignmentStatement*>(ref_tree);
		seal::impl::AssignmentStatement* parsed = static_cast<seal::impl::AssignmentStatement*>(parser_tree);
		if (ref->identificator != parsed->identificator) {
			return false;
		}
		return AreEqueal(ref->expression.get(), parsed->expression.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::WhileLoop)) {
		seal::impl::WhileLoop* ref = static_cast<seal::impl::WhileLoop*>(ref_tree);
		seal::impl::WhileLoop* parsed = static_cast<seal::impl::WhileLoop*>(parser_tree);
		return AreEqueal(ref->logics.get(), parsed->logics.get()) && AreEqueal(ref->body.get(), parsed->body.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::IfStatement)) {
		seal::impl::IfStatement* ref = static_cast<seal::impl::IfStatement*>(ref_tree);
		seal::impl::IfStatement* parsed = static_cast<seal::impl::IfStatement*>(parser_tree);
		return AreEqueal(ref->logics.get(), parsed->logics.get()) && AreEqueal(ref->body.get(), parsed->body.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::PrintStatement)) {
		seal::impl::PrintStatement* ref = static_cast<seal::impl::PrintStatement*>(ref_tree);
		seal::impl::PrintStatement* parsed = static_cast<seal::impl::PrintStatement*>(parser_tree);
		return AreEqueal(ref->variable.get(), parsed->variable.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::ReadStatement)) {
		seal::impl::ReadStatement* ref = static_cast<seal::impl::ReadStatement*>(ref_tree);
		seal::impl::ReadStatement* parsed = static_cast<seal::impl::ReadStatement*>(parser_tree);
		if (ref->variable_name != parsed->variable_name) {
			return false;
		}
		return true;
	} else if (typeid(*ref_tree) == typeid(seal::impl::Expression)) {
		seal::impl::Expression* ref = static_cast<seal::impl::Expression*>(ref_tree);
		seal::impl::Expression* parsed = static_cast<seal::impl::Expression*>(parser_tree);
		return AreEqueal(ref->expression.get(), parsed->expression.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::LogicsExpression)) {
		seal::impl::LogicsExpression* ref = static_cast<seal::impl::LogicsExpression*>(ref_tree);
		seal::impl::LogicsExpression* parsed = static_cast<seal::impl::LogicsExpression*>(parser_tree);
		if (ref->logics_operation.size() != parsed->logics_operation.size()) {
			return false;
		}
		for (std::size_t index = 0; index < ref->logics_operation.size(); index++) {
			if (ref->logics_operation[index] != parsed->logics_operation[index]) {
				return false;
			}
		}
		return AreEqueal(ref->expression1.get(), parsed->expression1.get()) && AreEqueal(ref->expression2.get(), parsed->expression2.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::Additive)) {
		seal::impl::Additive* ref = static_cast<seal::impl::Additive*>(ref_tree);
		seal::impl::Additive* parsed = static_cast<seal::impl::Additive*>(parser_tree);
		if (ref->operation != parsed->operation) {
			return false;
		}
		return AreEqueal(ref->right.get(), parsed->right.get()) && AreEqueal(ref->left.get(), parsed->left.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::Multiplicative)) {
		seal::impl::Multiplicative* ref = static_cast<seal::impl::Multiplicative*>(ref_tree);
		seal::impl::Multiplicative* parsed = static_cast<seal::impl::Multiplicative*>(parser_tree);
		if (ref->operation != parsed->operation) {
			return false;
		}
		return AreEqueal(ref->right.get(), parsed->right.get()) && AreEqueal(ref->left.get(), parsed->left.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::Unary)) {
		seal::impl::Unary* ref = static_cast<seal::impl::Unary*>(ref_tree);
		seal::impl::Unary* parsed = static_cast<seal::impl::Unary*>(parser_tree);
		if (ref->operation != parsed->operation) {
			return false;
		}
		return AreEqueal(ref->right.get(), parsed->right.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::Factor)) {
		seal::impl::Factor* ref = static_cast<seal::impl::Factor*>(ref_tree);
		seal::impl::Factor* parsed = static_cast<seal::impl::Factor*>(parser_tree);
		return AreEqueal(ref->value.get(), parsed->value.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::Brackets)) {
		seal::impl::Brackets* ref = static_cast<seal::impl::Brackets*>(ref_tree);
		seal::impl::Brackets* parsed = static_cast<seal::impl::Brackets*>(parser_tree);
		return AreEqueal(ref->expression.get(), parsed->expression.get());
	} else if (typeid(*ref_tree) == typeid(seal::impl::Literal)) {
		seal::impl::Literal* ref = static_cast<seal::impl::Literal*>(ref_tree);
		seal::impl::Literal* parsed = static_cast<seal::impl::Literal*>(parser_tree);
		if (ref->value != parsed->value) {
			return false;
		}
		return true;
	} else if (typeid(*ref_tree) == typeid(seal::impl::Identificator)) {
		seal::impl::Identificator* ref = static_cast<seal::impl::Identificator*>(ref_tree);
		seal::impl::Identificator* parsed = static_cast<seal::impl::Identificator*>(parser_tree);
		if (ref->name != parsed->name && ref->line != parsed->line) {
			return false;
		}
		return true;
	}
}

TEST_F(ParserTest, NewLineInputCheck) {
	EXPECT_EQ(parser.ParseProgram(), nullptr); // empty token vector
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	parser = { tokens };
	EXPECT_EQ(parser.ParseProgram(), nullptr); // vector with new line token only
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	parser = { tokens };
	std::vector<std::unique_ptr<seal::impl::ASTNode>> vect;
	vect.push_back(std::make_unique<seal::impl::Statement>((std::make_unique<seal::impl::ReadStatement>("fish"))));
	std::unique_ptr<seal::impl::ASTNode> ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // vector without new line token at the end
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "oyster", 2});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // no new line token between statements
}

TEST_F(ParserTest, ReadStatementCheck) {
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // double key word input
	tokens.pop_back();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to read literal
	tokens.pop_back();
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // key word without argument
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_right, "", 1});
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to read variable in brackets
}

TEST_F(ParserTest, ExpressionCheck) {
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::spit, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	parser = { tokens };
	std::vector<std::unique_ptr<seal::impl::ASTNode>> vect;
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::AssignmentStatement>("fish", std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Additive>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(2))), '+', std::make_unique<seal::impl::Unary>('-', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))))))));
	std::unique_ptr<seal::impl::ASTNode> ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // computing expression with a negative number
	tokens.clear();
	vect.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::spit, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "oyster", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute expression with a negative variable
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute expression with a key word
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::spit, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute expression with a negative nullptr
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::spit, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_right, "", 1});
	parser = { tokens };
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::AssignmentStatement>("fish", std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Additive>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))), '+', std::make_unique<seal::impl::Unary>('-', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Brackets> (std::make_unique<seal::impl::Additive>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(2))), '+', std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))))))))))));
	ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // computing expression with a negative brackets expression
	tokens.clear();
	vect.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_right, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_right, "", 1});
	parser = { tokens };
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::AssignmentStatement>("fish", std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Additive>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))), '+', std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Brackets>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Brackets>(std::make_unique<seal::impl::Additive>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(2))), '+', std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1)))))))))))))));
	ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // computing expression with a double brackets expression
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_right, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_right, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute expression with doubled brackets expressions
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_right, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute expression with empty brackets 
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute expression with left bracket only
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_right, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute expression with right bracket only
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute incomplete expression 
	tokens.clear();
	vect.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_right, "", 1});
	parser = { tokens };
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::AssignmentStatement>("fish", std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Brackets>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))))))))));
	ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // computing in-brackets number
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute brackets expression without right bracket
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute expression with double operation sign
	tokens.clear();
	vect.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::spit, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	parser = { tokens };
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::AssignmentStatement>("fish", std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('-', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1)))))));
	ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // computing single negative number
}

TEST_F(ParserTest, LogicsExpressionCheck) {
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::nah, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	std::vector<std::unique_ptr<seal::impl::ASTNode>> vect;
	std::vector<std::unique_ptr<seal::impl::ASTNode>> body;
	std::vector<char> operation;
	body.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::ReadStatement>("fish")));
	operation.push_back('!');
	operation.push_back('=');
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::IfStatement>(std::make_unique<seal::impl::LogicsExpression>(std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1)))), std::move(operation), std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))))), std::make_unique<seal::impl::StatementList>(std::move(body)))));
	std::unique_ptr<seal::impl::ASTNode> ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // computing !=
	tokens.clear();
	vect.clear();
	body.clear();
	operation.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::small, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	body.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::ReadStatement>("fish")));
	operation.push_back('<');
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::IfStatement>(std::make_unique<seal::impl::LogicsExpression>(std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1)))), std::move(operation), std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))))), std::make_unique<seal::impl::StatementList>(std::move(body)))));
	ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // computing <
	tokens.clear();
	vect.clear();
	body.clear();
	operation.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::small, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	body.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::ReadStatement>("fish")));
	operation.push_back('<');
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::IfStatement>(std::make_unique<seal::impl::LogicsExpression>(std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1)))), std::move(operation), std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))))), std::make_unique<seal::impl::StatementList>(std::move(body)))));
	ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // computing <
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute double key word
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::small, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "oyster", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute invalid logics expression with a key word
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::small, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute incomplete logics expression 
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to compute incomplete logics expression 
}

TEST_F(ParserTest, AssignmentStatementCheck) {
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // empty assign statement 
	tokens.clear(); 
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::nah, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to use logic expression for assignment 
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "2", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to assign number 
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::spit, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::is, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to assign negative variable
}

TEST_F(ParserTest, PrintStatementCheck) {
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bark, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bark, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // invalid command input: double key word
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bark, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_left, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bracket_right, "", 1});
	parser = { tokens };
	std::vector<std::unique_ptr<seal::impl::ASTNode>> vect;
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::PrintStatement>(std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Brackets>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Identificator>("fish", 1))))))))));
	std::unique_ptr<seal::impl::ASTNode> ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // printing in-brackets variable
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bark, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "oyster", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // trying to print logical expression
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::bark, "", 1});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // empty printing
}

TEST_F(ParserTest, ConditionsCheck) {
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // condition without logics expression
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::eat, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // condition with arithmetic expression
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // condition statement without body
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // condition statement without body and new line between logics and end key word
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	std::vector<std::unique_ptr<seal::impl::ASTNode>> vect;
	std::vector<std::unique_ptr<seal::impl::ASTNode>> body;
	std::vector<char> operation;
	body.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::ReadStatement>("fish")));
	operation.push_back('=');
	operation.push_back('=');
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::IfStatement>(std::make_unique<seal::impl::LogicsExpression>(std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1)))), std::move(operation), std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))))), std::make_unique<seal::impl::StatementList>(std::move(body)))));
	std::unique_ptr<seal::impl::ASTNode> ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // condition statement without new line between body and end key word -> Despite the fact that each line must be written at new line I decided that it is fine to write sneeze key word at the same line with body statement
	tokens.clear();
	vect.clear();
	body.clear();
	operation.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // new statement after sneeze key word without new line
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 3});
	parser = { tokens };
	EXPECT_THROW(parser.ParseProgram(), seal::impl::Exception); // condition statement without new line between logics and body
	tokens.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 3});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 3});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 3});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 4});
	parser = { tokens };
	body.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::ReadStatement>("fish")));
	operation.push_back('=');
	operation.push_back('=');
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::IfStatement>(std::make_unique<seal::impl::LogicsExpression>(std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1)))), std::move(operation), std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))))), std::make_unique<seal::impl::StatementList>(std::move(body)))));
	ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // condition statement with double new line between logics and body
	tokens.clear();
	vect.clear();
	body.clear();
	operation.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 3});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 4});
	parser = { tokens };
	body.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::ReadStatement>("fish")));
	operation.push_back('=');
	operation.push_back('=');
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::IfStatement>(std::make_unique<seal::impl::LogicsExpression>(std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1)))), std::move(operation), std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))))), std::make_unique<seal::impl::StatementList>(std::move(body)))));
	ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // condition statement with double new line between body and end key word
	tokens.clear();
	vect.clear();
	body.clear();
	operation.clear();
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::slap, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::tasty, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::literal, "1", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 1});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::snif, "", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::identificator, "fish", 2});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::new_line, "", 3});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 4});
	tokens.push_back(seal::impl::Token{seal::impl::TokenType::sneeze, "", 4});
	parser = { tokens };
	body.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::ReadStatement>("fish")));
	operation.push_back('=');
	operation.push_back('=');
	vect.push_back(std::make_unique<seal::impl::Statement>(std::make_unique<seal::impl::IfStatement>(std::make_unique<seal::impl::LogicsExpression>(std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1)))), std::move(operation), std::make_unique<seal::impl::Expression>(std::make_unique<seal::impl::Unary>('0', std::make_unique<seal::impl::Factor>(std::make_unique<seal::impl::Literal>(1))))), std::make_unique<seal::impl::StatementList>(std::move(body)))));
	ref_tree = std::make_unique<seal::impl::StatementList>(std::move(vect));
	EXPECT_EQ(AreEqueal(ref_tree.get(), parser.ParseProgram().get()), true); // condition statement with double end key word
}