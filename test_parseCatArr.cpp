#include "rand_story.hpp"

int main (int argc, char** argv) {
    std::ifstream word_file(argv[1]);
    std::map<std::string, std::set<std::string> > catArr;
    catArr = parseCatArr<std::string>(word_file);
    printWords(catArr);
    return EXIT_SUCCESS;
}