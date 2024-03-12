#pragma once

#include <iostream>
#include <optional>
#include <vector>
#include <string>

#include "lexer.h"
#include "context.h"
	
namespace seal {
	namespace impl {
		
		class Interpreter;

		class ASTNode {
		public:
			virtual ~ASTNode() = default;
			virtual std::optional<Value> Accept(Interpreter& i) = 0;
		};

		class StatementList : public ASTNode {
		public:
			StatementList(std::vector<std::unique_ptr<ASTNode>>&& nl);
			std::optional<Value> Accept(Interpreter& i) override;
			std::vector<std::unique_ptr<ASTNode>> node_list;
		};

		class Statement : public ASTNode {
		public:
			Statement(std::unique_ptr<ASTNode>&& n);
			std::optional<Value> Accept(Interpreter& i) override;
			std::unique_ptr<ASTNode> node;
		};

		class AssignmentStatement : public ASTNode {
		public:
			AssignmentStatement(const std::string& i, std::unique_ptr<ASTNode>&& e);
			std::optional<Value> Accept(Interpreter& i) override;
			std::string identificator;
			std::unique_ptr<ASTNode> expression;
		};

		class WhileLoop : public ASTNode {
		public:
			WhileLoop(std::unique_ptr<ASTNode>&& l, std::unique_ptr<ASTNode>&& b);
			std::optional<Value> Accept(Interpreter& i) override;
			std::unique_ptr<ASTNode> logics;
			std::unique_ptr<ASTNode> body;
		};

		class IfStatement : public ASTNode {
		public:
			IfStatement(std::unique_ptr<ASTNode>&& l, std::unique_ptr<ASTNode>&& b);
			std::optional<Value> Accept(Interpreter& i) override;
			std::unique_ptr<ASTNode> logics;
			std::unique_ptr<ASTNode> body;
		};

		class PrintStatement : public ASTNode {
		public:
			PrintStatement(std::unique_ptr<ASTNode>&& n);
			std::optional<Value> Accept(Interpreter& i) override;
			std::unique_ptr<ASTNode> variable;
		};

		class ReadStatement : public ASTNode {
		public:
			ReadStatement(const std::string& n);
			std::optional<Value> Accept(Interpreter& i) override;
			std::string variable_name;
		};

		class Expression : public ASTNode {
		public:
			Expression(std::unique_ptr<ASTNode>&& n);
			std::optional<Value> Accept(Interpreter& i) override;
			std::unique_ptr<ASTNode> expression;
		};

		class LogicsExpression : public ASTNode {
		public:
			LogicsExpression(std::unique_ptr<ASTNode>&& e1, std::vector<char>&& lo, std::unique_ptr<ASTNode>&& e2);
			std::optional<Value> Accept(Interpreter& i) override;
			std::unique_ptr<ASTNode> expression1;
			std::vector<char> logics_operation;
			std::unique_ptr<ASTNode> expression2;
		};

		class Additive : public ASTNode {
		public:
			Additive(std::unique_ptr<ASTNode>&& l, const char o, std::unique_ptr<ASTNode>&& r);
			std::optional<Value> Accept(Interpreter& i) override;
			std::unique_ptr<ASTNode> left;
			char operation;
			std::unique_ptr<ASTNode> right;
		};

		class Multiplicative : public ASTNode {
		public:
			Multiplicative(std::unique_ptr<ASTNode>&& l, const char o, std::unique_ptr<ASTNode>&& r);
			std::optional<Value> Accept(Interpreter& i) override;
			std::unique_ptr<ASTNode> left;
			char operation;
			std::unique_ptr<ASTNode> right;
		};

		class Unary : public ASTNode {
		public:
			Unary(const char o, std::unique_ptr<ASTNode>&& r);
			std::optional<Value> Accept(Interpreter& i) override;
			char operation;
			std::unique_ptr<ASTNode> right;
		};

		class Factor : public ASTNode {
		public:
			Factor(std::unique_ptr<ASTNode>&& n);
			std::optional<Value> Accept(Interpreter& i) override;
			std::unique_ptr<ASTNode> value;
		};

		class Brackets : public ASTNode {
		public:
			Brackets(std::unique_ptr<ASTNode>&& n);
			std::optional<Value> Accept(Interpreter& i) override;
			std::unique_ptr<ASTNode> expression;
		};

		class Literal : public ASTNode {
		public:
			Literal(int v);
			std::optional<Value> Accept(Interpreter& i) override;
			int value;
		};

		class Identificator : public ASTNode {
		public:
			Identificator(std::string n, std::size_t l);
			std::optional<Value> Accept(Interpreter& i) override;
			std::string name;
			std::size_t line;
		};

		class Parser {
		public:
			Parser(std::vector<Token> vec);
			std::unique_ptr<ASTNode> ParseProgram();
		private:
			std::unique_ptr<ASTNode> ParseStatementList();
			std::unique_ptr<ASTNode> ParseStatement();
			std::unique_ptr<ASTNode> ParseAssignmentStatement();
			std::unique_ptr<ASTNode> ParseWhileLoop();
			std::unique_ptr<ASTNode> ParseIfStatement();
			std::unique_ptr<ASTNode> ParsePrintStatement();
			std::unique_ptr<ASTNode> ParseReadStatement();
			std::unique_ptr<ASTNode> ParseExpression();
			std::unique_ptr<ASTNode> ParseLogicsExpression();
			std::unique_ptr<ASTNode> ParseAdditive();
			std::unique_ptr<ASTNode> ParseMultiplicative();
			std::unique_ptr<ASTNode> ParseUnary();
			std::unique_ptr<ASTNode> ParseFactor();
			std::unique_ptr<ASTNode> ParseLiteral();
			std::unique_ptr<ASTNode> ParseIdentificator();
			ErrorManager em;
			std::vector<Token> tokens;
			std::size_t current_token_index;
		};
	}
}
