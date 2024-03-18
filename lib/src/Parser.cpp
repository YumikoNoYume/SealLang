#include "Parser.h"
#include "Interpreter.h"

namespace seal {
	namespace impl {

		StatementList::StatementList(std::vector<std::unique_ptr<ASTNode>>&& nl) {
			auto it = nl.begin();
			for (it; it < nl.end(); it++) {
				node_list.emplace_back(std::move(*it));
			}
		}

		std::optional<Value> StatementList::Accept(Interpreter& i) {
			i.VisitStatementList(*this);
			return std::nullopt;
		}

		Statement::Statement(std::unique_ptr<ASTNode>&& n) : node(std::move(n)) {}

		std::optional<Value> Statement::Accept(Interpreter& i) {
			i.VisitStatement(*this);
			return std::nullopt;
		}

		AssignmentStatement::AssignmentStatement(const std::string& i, std::unique_ptr<ASTNode>&& e) : identificator(i), expression(std::move(e)) {}
		
		std::optional<Value> AssignmentStatement::Accept(Interpreter& i) {
			i.VisitAssignmentStatement(*this);
			return std::nullopt;
		}

		WhileLoop::WhileLoop(std::unique_ptr<ASTNode>&& l, std::unique_ptr<ASTNode>&& b) : logics(std::move(l)), body(std::move(b)) {}
		
		std::optional<Value> WhileLoop::Accept(Interpreter& i) {
			i.VisitWhileLoop(*this);
			return std::nullopt;
		}

		IfStatement::IfStatement(std::unique_ptr<ASTNode>&& l, std::unique_ptr<ASTNode>&& b) : logics(std::move(l)), body(std::move(b)) {}
		
		std::optional<Value> IfStatement::Accept(Interpreter& i) {
			i.VisitIfStatement(*this);
			return std::nullopt;
		}

		PrintStatement::PrintStatement(std::unique_ptr<ASTNode>&& n) : variable(std::move(n)) {}
		
		std::optional<Value> PrintStatement::Accept(Interpreter& i) {
			i.VisitPrintStatement(*this);
			return std::nullopt;
		}

		ReadStatement::ReadStatement(const std::string& n) : variable_name(n) {}
		
		std::optional<Value> ReadStatement::Accept(Interpreter& i) {
			i.VisitReadStatement(*this);
			return std::nullopt;
		}

		Expression::Expression(std::unique_ptr<ASTNode>&& n) : expression(std::move(n)) {}
		
		std::optional<Value> Expression::Accept(Interpreter& i) {
			return i.VisitExpression(*this);
		}

		LogicsExpression::LogicsExpression(std::unique_ptr<ASTNode>&& e1, std::vector<char>&& lo, std::unique_ptr<ASTNode>&& e2) : expression1(std::move(e1)), logics_operation(std::move(lo)), expression2(std::move(e2)) {}
		
		std::optional<Value> LogicsExpression::Accept(Interpreter& i) {
			return i.VisitLogicsExpression(*this);
		}

		Additive::Additive(std::unique_ptr<ASTNode>&& l, const char o, std::unique_ptr<ASTNode>&& r) : left(std::move(l)), operation(o), right(std::move(r)) {}
		
		std::optional<Value> Additive::Accept(Interpreter& i) {
			return i.VisitAdditive(*this);
		}

		Multiplicative::Multiplicative(std::unique_ptr<ASTNode>&& l, const char o, std::unique_ptr<ASTNode>&& r) : left(std::move(l)), operation(o), right(std::move(r)) {}
		
		std::optional<Value> Multiplicative::Accept(Interpreter& i) {
			return i.VisitMultiplicative(*this);
		}

		Unary::Unary(const char o, std::unique_ptr<ASTNode>&& r) : operation(o), right(std::move(r)) {}
		
		std::optional<Value> Unary::Accept(Interpreter& i) {
			return i.VisitUnary(*this);
		}

		Factor::Factor(std::unique_ptr<ASTNode>&& n) : value(std::move(n)) {}
		
		std::optional<Value> Factor::Accept(Interpreter& i) {
			return i.VisitFactor(*this);
		}

		Brackets::Brackets(std::unique_ptr<ASTNode>&& n) : expression(std::move(n)) {}
		
		std::optional<Value> Brackets::Accept(Interpreter& i) {
			return i.VisitBrackets(*this);
		}

		Literal::Literal(int v) : value(v) {}
		
		std::optional<Value> Literal::Accept(Interpreter& i) {
			return i.VisitLiteral(*this);
		}

		Identificator::Identificator(std::string n, std::size_t l) : name(n), line(l) {}
		
		std::optional<Value> Identificator::Accept(Interpreter& i) {
			return i.VisitIdentificator(*this);
		}

		Parser::Parser(std::vector<Token> vec) : current_token_index(0) {
			std::swap(tokens, vec);
		}

		std::unique_ptr<ASTNode> Parser::ParseProgram() {
			if (tokens.size() == 0 || (tokens.size() == 1 && tokens.back().type == TokenType::new_line)) {
				return nullptr;
			}
			else {
				if (tokens.back().type != TokenType::new_line) {
					tokens.push_back(Token{ TokenType::new_line, "", tokens.back().line });
				}
				if (tokens[0].type == TokenType::new_line) {
					current_token_index++;
				}
				std::unique_ptr<ASTNode> program = ParseStatementList();
				if (em.GetErrorListSize() > 0) {
					throw (Exception(std::move(em.GetErrorMessages())));
				}
				return program;
			}
		}

		std::unique_ptr<ASTNode> Parser::ParseStatementList() {
			std::unique_ptr<ASTNode> statement;
			std::vector<std::unique_ptr<ASTNode>> list;
			if (tokens[current_token_index].type != TokenType::sneeze && tokens[current_token_index].type != TokenType::new_line) { 
				statement = ParseStatement(); 
				list.emplace_back(std::move(statement));
			}
			while (tokens[current_token_index].type == TokenType::new_line && current_token_index < tokens.size() - 1) {
				current_token_index++;
				if (tokens[current_token_index].type == TokenType::sneeze || tokens[current_token_index].type == TokenType::new_line) continue;
				if (current_token_index < tokens.size()) {
					statement = ParseStatement();
					list.emplace_back(std::move(statement));
				}
				else {
					return std::make_unique<StatementList>(std::move(list));
				}
			}
			if (tokens[current_token_index].type == TokenType::sneeze) {
				current_token_index++;
				if (list.size() == 0) {
					em.CollectError(Error(tokens[current_token_index].line, 16));
					return nullptr;
				}
				return std::make_unique<StatementList>(std::move(list));
			}
			if (current_token_index != tokens.size() - 1) {
				em.CollectError(Error(tokens[current_token_index].line, 13));
				return nullptr;
			}
			return std::make_unique<StatementList>(std::move(list));
		}

		std::unique_ptr<ASTNode> Parser::ParseStatement() {
			switch (tokens[current_token_index].type) {
			case TokenType::snif:
				return std::make_unique<Statement>(ParseReadStatement());
			case TokenType::bark:
				return std::make_unique<Statement>(ParsePrintStatement());
			case TokenType::bouncing:
				return std::make_unique<Statement>(ParseWhileLoop());
			case TokenType::slap:
				return std::make_unique<Statement>(ParseIfStatement());
			case TokenType::identificator:
				if (tokens[current_token_index + 1].type == TokenType::is) {
					return std::make_unique<Statement>(ParseAssignmentStatement());
				}
			default: 
				em.CollectError(Error(tokens[current_token_index].line, 6));
				return nullptr;
			}
		}

		std::unique_ptr<ASTNode> Parser::ParseAssignmentStatement() {
			std::string name = tokens[current_token_index].lexeme;
			current_token_index += 2;
			std::unique_ptr<ASTNode> ex = ParseExpression();
			return std::make_unique<AssignmentStatement>(name, std::move(ex));
		}

		std::unique_ptr<ASTNode> Parser::ParseWhileLoop() {
			current_token_index++;
			std::unique_ptr<ASTNode> logics = ParseLogicsExpression();
			if (tokens[current_token_index].type != TokenType::new_line) {
				em.CollectError(Error(tokens[current_token_index].line, 9));
				return nullptr;
			}
			else {
				current_token_index++;
				std::unique_ptr<ASTNode> body = ParseStatementList();
				return std::make_unique<WhileLoop>(std::move(logics), std::move(body));
			}

		}

		std::unique_ptr<ASTNode> Parser::ParseIfStatement() {
			current_token_index++;
			std::unique_ptr<ASTNode> logics = ParseLogicsExpression();
			if (tokens[current_token_index].type != TokenType::new_line) {
				em.CollectError(Error(tokens[current_token_index].line, 9));
				return nullptr;
			}
			else {
				current_token_index++;
				std::unique_ptr<ASTNode> body = ParseStatementList();
				return std::make_unique<IfStatement>(std::move(logics), std::move(body));
			}

		}

		std::unique_ptr<ASTNode> Parser::ParsePrintStatement() {
			current_token_index++;
			return std::make_unique<PrintStatement>(ParseExpression());
		}

		std::unique_ptr<ASTNode> Parser::ParseReadStatement() {
			if (tokens[current_token_index + 1].type != TokenType::identificator) {
				em.CollectError(Error(tokens[current_token_index].line, 3));
				return nullptr;
			}
			else {
				std::string name = tokens[current_token_index + 1].lexeme;
				current_token_index += 2;
				return std::make_unique<ReadStatement>(name);
			}
		}

		std::unique_ptr<ASTNode> Parser::ParseExpression() {
			if (tokens[current_token_index].type != TokenType::identificator && tokens[current_token_index].type != TokenType::literal && tokens[current_token_index].type != TokenType::bracket_left && tokens[current_token_index].type != TokenType::spit) {
				em.CollectError(Error(tokens[current_token_index].line, 5));
				return nullptr;
			}
			else {
				return std::make_unique<Expression>(ParseAdditive());
			}
		}

		std::unique_ptr<ASTNode> Parser::ParseLogicsExpression() {
			std::unique_ptr<ASTNode> ex1 = ParseExpression();
			std::vector<char> lop;
			if (tokens[current_token_index].type != TokenType::small && tokens[current_token_index].type != TokenType::big &&
				tokens[current_token_index].type != TokenType::nah && tokens[current_token_index].type != TokenType::tasty) {
				em.CollectError(Error(tokens[current_token_index].line, 4));
				return nullptr;
			}
			else if (tokens[current_token_index].type == TokenType::nah) {
				lop.push_back('!');
				current_token_index++;
			}
			switch (tokens[current_token_index].type) {
			case TokenType::small:
				lop.push_back('<'); break;
			case TokenType::big:
				lop.push_back('>'); break;
			case TokenType::tasty:
				lop.push_back('=');
				if (lop.size() < 2) {
					lop.push_back('=');
				}
			}
			current_token_index++;
			std::unique_ptr<ASTNode> ex2 = ParseExpression();
			return std::make_unique<LogicsExpression>(std::move(ex1), std::move(lop), std::move(ex2));
		}

		std::unique_ptr<ASTNode> Parser::ParseAdditive() {
			std::unique_ptr<ASTNode> val = ParseMultiplicative();
			while (tokens[current_token_index].type == TokenType::eat || tokens[current_token_index].type == TokenType::spit) {
				char op = tokens[current_token_index].type == TokenType::eat ? '+' : '-';
				current_token_index++;
				if (current_token_index >= tokens.size() - 1) {
					em.CollectError(Error(tokens[current_token_index].line, 2));
					return nullptr;
				}
				std::unique_ptr<ASTNode> expr = std::make_unique<Additive>(std::move(val), op, ParseMultiplicative());
				val = std::move(expr);
			}
			return val;
		}

		std::unique_ptr<ASTNode> Parser::ParseMultiplicative() {
			std::unique_ptr<ASTNode> val = ParseUnary();
			while (tokens[current_token_index].type == TokenType::imagine || tokens[current_token_index].type == TokenType::bite) {
				char op = tokens[current_token_index].type == TokenType::imagine ? '*' : '/';
				current_token_index++;
				if (current_token_index >= tokens.size() - 1) {
					em.CollectError(Error(tokens[current_token_index].line, 2));
					return nullptr;
				}
				std::unique_ptr<ASTNode> expr = std::make_unique<Multiplicative>(std::move(val), op, ParseUnary());
				val = std::move(expr);
			}
			return val;
		}

		std::unique_ptr<ASTNode> Parser::ParseUnary() {
			char op = '0';
			if (tokens[current_token_index].type == TokenType::spit && (tokens[current_token_index + 1].type == TokenType::literal || tokens[current_token_index + 1].type == TokenType::bracket_left)) {
				op = '-';
				current_token_index++;
			}
			else if (tokens[current_token_index].type == TokenType::spit && tokens[current_token_index + 1].type == TokenType::identificator) {
				em.CollectError(Error(tokens[current_token_index].line, 14));
				return nullptr;
			}
			return std::make_unique<Unary>(op, ParseFactor());
		}

		std::unique_ptr<ASTNode> Parser::ParseFactor() {
			std::unique_ptr<ASTNode> expr;
			switch (tokens[current_token_index].type) {
			case TokenType::literal:
				return std::make_unique<Factor>(ParseLiteral());
			case TokenType::identificator:
				return std::make_unique<Factor>(ParseIdentificator());
			case TokenType::bracket_left:
				if (current_token_index + 1 >= tokens.size() - 1) {
					em.CollectError(Error(tokens[current_token_index].line, 12));
					return nullptr;
				}
				current_token_index++;
				expr = std::make_unique<Brackets>(ParseAdditive());
				if (tokens[current_token_index].type != TokenType::bracket_right) {
					em.CollectError(Error(tokens[current_token_index].line, 8));
					return nullptr;
				}
				else {
					current_token_index++;
					return std::make_unique<Factor>(std::move(expr));
				}
			default: 
				em.CollectError(Error(tokens[current_token_index].line, 15));
				current_token_index++;
				return nullptr;
			}
		}

		std::unique_ptr<ASTNode> Parser::ParseLiteral() {
			current_token_index++;
			return std::make_unique<Literal>(std::stoi(tokens[current_token_index - 1].lexeme));
		}

		std::unique_ptr<ASTNode> Parser::ParseIdentificator() {
			current_token_index++;
			return std::make_unique<Identificator>(tokens[current_token_index - 1].lexeme, tokens[current_token_index - 1].line);
		}

	}
}
