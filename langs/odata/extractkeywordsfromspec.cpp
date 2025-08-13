#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <set>
#include <map>

std::set<std::string> keywords{};

bool isCommentLine(const std::string &line)
{
    return line.empty() || line[0] == ';';
}

void captureKeywords(const std::string &line)
{
    // This function captures keywords from the syntax line
    // For example, it could extract keywords or perform other operations
    std::regex pat("([\"'])(.*?)\\1");
    std::smatch match;

    for (auto it = std::sregex_iterator(line.begin(), line.end(), pat);
         it != std::sregex_iterator(); ++it)
    {
        std::smatch match = *it;
        // std::cout << match[1] << " => " << match[2] << "\n";
        keywords.insert(match[2]);
    }
}

void processSyntax(const std::string &line)
{
    // This function processes the syntax line
    // For example, it could extract keywords or perform other operations
    captureKeywords(line);
    // Here you would add logic to extract keywords or other relevant information
}

int main()
{
    std::fstream spec("odata-abnf-construction-rules.txt");
    for (std::string line; std::getline(spec, line);)
    {
        if (!isCommentLine(line))
        {
            processSyntax(line);
        }
    }
    spec.close();
    std::cout << "Extracted Keywords:\n";
    for (const auto &keyword : keywords)
    {
        std::cout << keyword << "\n";
    }
    return 0;
}
