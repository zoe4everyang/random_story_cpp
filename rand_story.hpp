#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>

void store_story_template();
std::string choose_word(std::string cat_name, std::map<std::string, std::set<std::string> > cat_array);
void store_story_template(std::istream & story_file);