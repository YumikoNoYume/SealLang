#include "ErrorManager.h"

namespace seal {
	namespace impl {
		
		void ErrorManager::CollectError(Error e) {
			error_list.push_back(e);
		}

		void ErrorManager::DispalyError() const {
			for (auto er : error_list) {
				std::cerr << "Error! " << er.message << "line: " << er.line << "\n";
			}
			exit(1);
		};

		std::size_t ErrorManager::GetErrorListSize() const {
			return error_list.size();
		}

		std::vector<std::string> ErrorManager::GetErrorMessages() const{
			std::vector<std::string> eml;
			for (auto e : error_list) {
				eml.push_back(e.message);
			}
			return eml;
		}

		Error::Error(std::size_t l, std::size_t c) : line(l), error_code(c) {
			message = SetErrorMessage(error_code);
			message.append(" Line: ");
			message.append(std::to_string(line));
		}

		void ErrorManager::Clear() {
			error_list.clear();
		}

		std::string Error::SetErrorMessage(std::size_t c) {
			switch (c) {
			case 1: return "Wrong lexeme input!";
			case 2: return "Incomplete arithmetic expression!";
			case 3: return "\"snif\" function requires variable!"; 
			case 4: return "Input value must be logics operator!";
			case 5: return "Input value must be variable or numeric!";
			case 6: return "Invalid statement input!";
			case 7: return "Input value must be arithmetic operator!"; 
			case 8: return "Excess brackets!";
			case 9: return "Wrong expression input! Must be ended with new line!"; 
			case 10: return "While loops' anf if statements' body must be ended with \"sneeze\"!";
			case 11: return "Unknown variable!"; 
			case 12: return "Brackets must contain expression!";
			case 13: return "Each statement must be written at new line!";
			case 14: return "Invalid variable input!";
			case 15: return "Invalid arithmetic expression input!";
			case 16: return "While loops' anf if statements must contain body!";
			case 17: return "Integer range is from -2.147.483.648 to 2.147.483.647!";
			}
		}
	}
}
