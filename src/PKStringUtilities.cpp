#include "PKStringUtilities.h"
#include <fstream>

namespace PK::Assets::StringUtilities
{
	std::string Trim(const std::string& value)
	{
		auto first = value.find_first_not_of(" \n\r");
	
		if (first == std::string::npos)
		{
			return value;
		}
	
		return value.substr(first, (value.find_last_not_of(" \n\r") - first + 1));
	}
	
	std::vector<std::string> Split(const std::string& value, const char* symbols)
	{
		std::vector<std::string> output;
	
		auto start = value.find_first_not_of(symbols, 0);
	
		while (start != std::string::npos)
		{
			auto end = value.find_first_of(symbols, start);
	
			if (end == std::string::npos)
			{
				output.push_back(value.substr(start));
				break;
			}
	
			output.push_back(value.substr(start, end - start));
			start = value.find_first_not_of(symbols, end);
		}
	
		return output;
	}
	
	std::string ReadFileName(const std::string& filepath)
	{
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		return filepath.substr(lastSlash, count);
	}
	
	std::string ReadDirectory(const std::string& filepath)
	{
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		return filepath.substr(0, lastSlash);
	}
	
	std::string ReadFileRecursiveInclude(const std::string& filepath, std::vector<std::string>& includes)
	{
		auto includeOnceToken = "#pragma once";
		auto includeToken = "#include ";
		auto includeTokenLength = strlen(includeToken);
		auto foundPragmaOnce = false;
	
		std::ifstream file(filepath, std::ios::in);
	
		if (!file)
		{
			return "FAILED TO OPEN: " + filepath;
		}
	
		std::string result;
		std::string lineBuffer;
	
		while (std::getline(file, lineBuffer))
		{
			if (!foundPragmaOnce && lineBuffer.find(includeOnceToken) != lineBuffer.npos)
			{
				foundPragmaOnce = true;
	
				if (std::find(includes.begin(), includes.end(), filepath) != includes.end())
				{
					file.close();
					return "";
				}
	
				includes.push_back(filepath);
				continue;
			}
	
			auto includepos = lineBuffer.find(includeToken);
	
			if (includepos != lineBuffer.npos)
			{
				lineBuffer.erase(0, includepos + includeTokenLength);
				lineBuffer.insert(0, filepath.substr(0, filepath.find_last_of("/\\") + 1));
				result += ReadFileRecursiveInclude(lineBuffer, includes);
				continue;
			}
	
			result += lineBuffer + '\n';
		}
	
		file.close();
	
		return result;
	}
	
	std::string ReadFileRecursiveInclude(const std::string& filepath)
	{
		std::vector<std::string> includes;
		return ReadFileRecursiveInclude(filepath, includes);
	}
	
	std::string ExtractToken(const char* token, std::string& source, bool includeToken)
	{
		std::string firstToken;
	
		auto pos = source.find(token, 0);
	
		while (pos != std::string::npos)
		{
			auto eol = source.find_first_of("\r\n", pos);
			auto sol = source.find_first_not_of("\r\n", eol);

			if (eol == std::string::npos ||
				sol == std::string::npos)
			{
				return firstToken;
			}
	
			if (firstToken.empty())
			{
				if (includeToken)
				{
					firstToken = source.substr(pos, sol - pos);
				}
				else
				{
					auto spos = pos + strlen(token);
					firstToken = source.substr(spos, eol - spos);
				}
			}
	
			source.erase(pos, sol - pos);
			pos = source.find(token);
		}
	
		return firstToken;
	}
	
	size_t ExtractToken(size_t offset, const char* token, std::string& source, std::string& ouput, bool includeToken)
	{
		auto pos = source.find(token, offset);
	
		if (pos == std::string::npos)
		{
			return std::string::npos;
		}
	
		auto eol = source.find_first_of("\r\n", pos);
		auto sol = source.find_first_not_of("\r\n", eol);

		if (eol == std::string::npos ||
			sol == std::string::npos)
		{
			return std::string::npos;
		}
	
		if (includeToken)
		{
			ouput = source.substr(pos, sol - pos);
		}
		else
		{
			auto spos = pos + strlen(token);
			ouput = source.substr(spos, eol - spos);
		}
	
		source.erase(pos, sol - pos);
		return pos;
	}
	
	void ExtractTokens(const char* token, std::string& source, std::vector<std::string>& tokens, bool includeToken)
	{
		auto pos = source.find(token, 0);
	
		while (pos != std::string::npos)
		{
			auto eol = source.find_first_of("\r\n", pos);
			auto sol = source.find_first_not_of("\r\n", eol);

			if (eol == std::string::npos ||
				sol == std::string::npos)
			{
				return;
			}

			if (includeToken)
			{
				tokens.push_back(source.substr(pos, sol - pos));
			}
			else
			{
				auto spos = pos + strlen(token);
				tokens.push_back(source.substr(spos, eol - spos));
			}
	
			source.erase(pos, sol - pos);
			pos = source.find(token);
		}
	}
	
	void FindTokens(const char* token, const std::string& source, std::vector<std::string>& tokens, bool includeToken)
	{
		auto pos = source.find(token, 0);
	
		while (pos != std::string::npos)
		{
			auto eol = source.find_first_of("\r\n", pos);
			auto sol = source.find_first_not_of("\r\n", eol);

			if (eol == std::string::npos ||
				sol == std::string::npos)
			{
				return;
			}
	
			if (includeToken)
			{
				tokens.push_back(source.substr(pos, sol - pos));
			}
			else
			{
				auto spos = pos + strlen(token);
				tokens.push_back(source.substr(spos, eol - spos));
			}
	
			pos = source.find(token, sol);
		}
	}
	
	size_t FirstIndexOf(const char* str, char c)
	{
		auto ptr = strchr(str, c);
		return ptr == nullptr ? std::string::npos : (ptr - str);
	}
	
	size_t LastIndexOf(const char* str, char c)
	{
		auto ptr = strrchr(str, c);
		return ptr == nullptr ? std::string::npos : (ptr - str);
	}
}