
#include <iostream>
#include <fstream>
#include <string>

int main()
{
    // 1) Back up the original buffers
    std::streambuf *old_cin_buf = std::cin.rdbuf();
    std::streambuf *old_cout_buf = std::cout.rdbuf();

    // 2) Open files for redirection
    std::ifstream file_in("input.txt");
    std::ofstream file_out("output.txt");
    if (!file_in || !file_out)
    {
        std::cerr << "Failed to open input.txt or output.txt\n";
        return 1;
    }

    // 3) Redirect cin/cout
    std::cin.rdbuf(file_in.rdbuf());
    std::cout.rdbuf(file_out.rdbuf());

    // 4) Use cin/cout as usual, now talking to files:
    //    Suppose input.txt contains:
    //      42 Hello
    int number;
    std::string word;
    std::cin >> number >> word;
    std::cout << "Read from input.txt: "
              << number << " and \"" << word << "\"\n";

    // 5) Restore original buffers
    std::cin.rdbuf(old_cin_buf);
    std::cout.rdbuf(old_cout_buf);

    // 6) Optionally, notify user on console
    std::cout << "Finished redirecting. "
              << "Check output.txt for the results.\n";

    return 0;
}
