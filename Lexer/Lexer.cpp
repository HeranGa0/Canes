#include"Lexer.h"

void Lexer::readLine()
{
	std::string line;
	if (!std::getline(reader, line)) {
		hasMore = false;
		return;
	}
	lineNumber++;
	std::string st=line;
	while (!st.empty()) {
		st=addToken(st);

	}
	queue.push_back(new IdToken(lineNumber, Token::EOL));
}

std::string Lexer::toStringLiteral(std::string s)
{
	std::string re;
	int len = (unsigned int)s.length()-1;
	for (int i = 1; i < len; i++) {
		char c = s.at(i);
		if (c == '\\' && i + 1 < len) {
			auto c2 = s.at(i + 1);
			if (c2 == '"' || c2 == '\\')
				c = s.at(++i);
			else if (c2 == 'n') {
				++i;
				c = '\n';
			}

			
		}
		re.push_back(c);
	}
	return re;
}

std::string Lexer::addToken(std::string current)
{
	std::smatch result;
	if (!std::regex_search(current, result, comment, std::regex_constants::match_continuous)) {
        if(std::regex_search(current, result, dnum, std::regex_constants::match_continuous)) {
            //std::cout << result[0] << std::endl << result.suffix() << std::endl;
            queue.push_back(new DnumToken(lineNumber, std::stod(result[0])));
        }

		else if (std::regex_search(current, result, num, std::regex_constants::match_continuous)) {
			queue.push_back(new NumToken(lineNumber, std::stoi(result[0])));
		}
		else if (std::regex_search(current, result, str, std::regex_constants::match_continuous)) {
			queue.push_back(new StrToken(lineNumber, toStringLiteral(trimming(result[0]))));
		}
		else if(std::regex_search(current, result, id, std::regex_constants::match_continuous)) {
			//std::cout << result[0] << std::endl << result.suffix() << std::endl;
			queue.push_back(new IdToken(lineNumber, trimming(result[0])));
		}
        
		return result.suffix();
	}
	return "";
}

