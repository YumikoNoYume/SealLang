#include "Exception.h"

namespace seal {
	namespace impl {
		Exception::Exception(std::vector<std::string>&& el) : exception_list(std::move(el)) {}

		std::vector<std::string> Exception::GetException() const {
			return exception_list;
		}
	}
}
