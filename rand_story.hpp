#include <map>
#include <set>
#include <utility> // pair
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>

void store_story_template();
std::string choose_word(std::string cat_name, std::map<std::string, std::set<std::string> > cat_array);
std::vector<std::string> store_story_template(std::istream & story_file);
void printWords(std::map<std::string, std::set<std::string> > ans);

// store word to category array
template <typename T>
std::map<T, std::set<T> > storeWords(T cateName, T word, std::map<T, std::set<T> > ans) {
    if (ans.count(cateName) == 0) { // category not exist
        std::set<T> cat;
        cat.insert(word);
        ans.insert(std::pair<T, std::set<T> >(cateName, cat));
    } else { // cat exist
        std::map<std::string, std::set<std::string> >::iterator it;
        it = ans.find(cateName);
        it->second.insert(word);
    }
    return ans;
}
// parse one line in wordFile, split category and word
template <typename T>
std::map<T, std::set<T> > parseLine(T lines, std::map<T, std::set<T> > ans) {
    std::size_t colon = lines.find(':');
    if (colon == T::npos) { // when string.find does not find ":", it will return npos
        fprintf(stderr, "There is one line without colon, or the file is not a wordFile.\n");
        exit(EXIT_FAILURE);
    }
    T catName = lines.substr(0, colon);
    
    std::size_t end = lines.find('\n', colon + 1);
    T word = lines.substr(colon + 1, end);
    if (word.find(':') != T::npos) {
        fprintf(stderr, "There is one line with more than one colons.\n");
        exit(EXIT_FAILURE);
    }
    
    ans = storeWords<T>(catName, word, ans);
    
    return ans;
}
// parse pairs in wordfile into a map<string, set<string> >
template <typename T>
std::map<T, std::set<T> > parseCatArr(std::ifstream & word_file) {
    std::map<T, std::set<T> > cat;
    T current;
    while (!word_file.eof()) {
        std::getline(word_file, current);
        if (current == "") {
            continue;
        }
        cat = parseLine<T>(current, cat); // parse every line in wordsFile
    }
    return cat;
}

