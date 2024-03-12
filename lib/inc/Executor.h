#pragma once

#include <vector>
#include <string>

#include "Interpreter.h"

namespace seal {
	class Executor {
	public:
		Executor(Context cont); 
		void Execute(const std::string& program);
	private:
		impl::Lexer lexer;
		impl::Parser parser;
		impl::Interpreter interpreter;
		std::vector <impl::Token> tokens;
	};
}
