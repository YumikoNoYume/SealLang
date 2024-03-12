#pragma once

#include <exception>
#include <iostream>
#include <vector>

namespace seal {
	namespace impl {
		class Exception : public std::exception {
		public:
			Exception(std::vector<std::string>&& el);
			std::vector<std::string> GetException() const;
		private:
			std::vector<std::string> exception_list;
		};
	}
}
