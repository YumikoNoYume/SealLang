#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "Exception.h"

namespace seal {
	namespace impl {
		
		class Error {
		public:
			Error(std::size_t l, std::size_t c);
			std::size_t line;
			std::string message;
			std::size_t error_code;
		private:
			static std::string SetErrorMessage(std::size_t code);
		};

		class ErrorManager {
		public:
			ErrorManager() = default;
			void CollectError(Error e);
			void DispalyError() const;
			std::size_t GetErrorListSize() const;
			std::vector<std::string> GetErrorMessages() const;
			void Clear();
		private:
			std::vector<Error> error_list;
		};

	}
}
