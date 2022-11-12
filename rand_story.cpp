#include "rand_story.hpp"

// helper function to print out the story template stored in vector<string>
void print_story(std::vector<std::string> story){
    auto it = story.begin();
    while(it != story.end()){
        std::cout << *it << " ";
        ++it;
    }
}

// store each line of story template into a vector<string> 
void store_story_template(std::istream & story_file){
    std::string line;
    std::vector<std::string> story_template; 
    while(!story_file.eof()){
        std::getline(story_file, line);
        story_template.push_back(line);
    }
    print_story(story_template);
}

// randomly choose a word from specified category "cat_name"
std::string choose_word(std::string cat_name, std::map<std::string, std::set<std::string> > cat_array){
    if (cat_array.find(cat_name) == cat_array.end()) {
        std::cerr << "Specified category name does not exist! " << std::endl;
        exit(EXIT_FAILURE);
    }
    std::set<std::string> curr_words = cat_array[cat_name];
    srand (time(NULL));
    int size = curr_words.size();
    int rand_num = rand() % size;
    auto it = curr_words.begin();
    //it = it + rand_num;
    for(int i = 0; i < rand_num && it != curr_words.end(); i++){
        ++it;
    }
    return *it;
}




// main function for zoe's part (store_story_template) for now
int main(int argc, char** argv){
    if(argc != 2){
        std::cerr << "wrong number of arguments" << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream story_file(argv[1]);
    if(!story_file.is_open()){
        std::cerr << "failed to read file" << std::endl;
    }
    // store each line of story file into a vector<string> and print out
    store_story_template(story_file);

    // create a "test_cat_array" map with one category "fruit" for testing
    std::map<std::string, std::set<std::string> > test_cat_array;
    std::set<std::string> test_words;
    test_words.insert("Apple");
    test_words.insert("Pear");
    test_words.insert("Orange");
    test_cat_array["fruit"] = test_words;

    // randomly choose a word from specified category and print out
    std::string cat_name = "animal";
    std::string rand_word = choose_word(cat_name, test_cat_array);
    std::cout << std::endl;
    std::cout << "Choose random word from " << cat_name << ": " << rand_word << std::endl;


    return EXIT_SUCCESS;
}
