#include "Executor.h"

namespace seal {
	
	Executor::Executor(Context cont) : parser(tokens), interpreter(cont) {}
	
	void Executor::Execute(const std::string& program) {
		tokens.clear();
		lexer.ConvertStringToTokens(program, tokens);
		parser = {tokens};
		std::unique_ptr<seal::impl::ASTNode> prog = parser.ParseProgram();
		prog->Accept(interpreter);
	}
}
