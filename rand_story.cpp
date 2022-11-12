#include "p13.hpp"

// helper function to print out the story template stored in vector<string>
void print_story(std::vector<std::string> story)
{
    std::vector<std::string>::iterator it = story.begin();
    while (it != story.end())
    {
        std::cout << *it;
        ++it;
    }
}

// store each line of story template into a vector<string>
std::vector<std::string> store_story_template(std::istream &story_file)
{
    std::string line;
    std::vector<std::string> story_template;
    while (!story_file.eof())
    {
        std::getline(story_file, line);
        story_template.push_back(line);
    }
    // print_story(story_template);
    return story_template;
}

// randomly choose a word from specified category "cat_name"
std::string choose_word(std::string cat_name, std::map<std::string, std::set<std::string> > cat_array)
{
    std::set<std::string> curr_words = cat_array[cat_name];
    srand(time(NULL));
    int size = curr_words.size();
    int rand_num = rand() % size;
    std::set<std::string>::iterator it = curr_words.begin();
    // it = it + rand_num;
    for (int i = 0; i < rand_num && it != curr_words.end(); i++)
    {
        ++it;
    }
    return *it;
}
/*-------------------------------------------------------------------------
-------------   parse category : word pair into map   ---------------------
---------------------------------------------------------------------------*/
void printWords(std::map<std::string, std::set<std::string> > ans)
{
    std::map<std::string, std::set<std::string> >::iterator it_map;
    it_map = ans.begin();
    while (it_map != ans.end())
    {
        std::cout << " " << it_map->first << ": " << std::endl;
        std::set<std::string>::iterator it_set;
        it_set = it_map->second.begin();
        while (it_set != it_map->second.end())
        {
            std::cout << "\t" << *it_set << std::endl;
            ++it_set;
        }
        ++it_map;
    }
}

//help us find if the line exit _
// return 0 if exit
// return 1 if not
int Checkunderline(std::string &line)
{
    std::size_t pos1 = line.find("_");
    if (pos1 == std::string::npos)
    {
        return 0; //can not find _
    }
    return 1; //still exit _
}

//check if the name is only characters
// or an int
int checkname(std::string cat_name)
{
    int result = 0;
    size_t countInt = 0;
    for (std::size_t i = 0; i < cat_name.size(); i++)
    {
        if (!isalpha(cat_name[i]))
        {
            countInt++;
        }
    }
    // the string contains integers and chars
    if (countInt != 0 && countInt != cat_name.size())
    {
        std::cerr << "The input string is not filled with only characters" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (countInt != 0 && countInt == cat_name.size())
    { // string only contains integer
        result = 1;
    }
    return result;
}

// if the cat_name is an int, then use chooseInt to find a used word
std::string chooseInt(std::string cat_name, std::vector<std::string> &usedWords)
{
    size_t checkValue = atoi(cat_name.c_str());
    if (checkValue <= 0 || checkValue > usedWords.size())
    {
        std::cerr << "The integer in the string is invalid" << std::endl;
        exit(EXIT_FAILURE);
    }
    return usedWords[usedWords.size() - checkValue];
}

//replace the cat_name according to cat_array
//find the first _ to get the cat_name position
//replace one cat_name for each call
void doreplace(std::string &line, std::map<std::string, std::set<std::string> > cat_array, std::vector<std::string> &usedWords)
{
    std::size_t pos1 = line.find("_");
    std::size_t pos2_1 = line.find(" ", pos1 + 1);
    std::size_t pos2_2 = line.find(".", pos1 + 1);
    std::size_t pos2;
    if (pos2_1 == std::string::npos) // no " " after cat_name
    {
        pos2 = pos2_2;
    }
    else if (pos2_2 == std::string::npos) // no "." after cat_name
    {
        pos2 = pos2_1;
    }
    else //exit "." and " " then find the nearest one to determine the lenth of cat_name
    {
        if (pos2_2 >= pos2_1)
        {
            pos2 = pos2_1;
        }
        else
        {
            pos2 = pos2_2;
        }
    }
    if (pos2 == std::string::npos) //the cat_name is the last word of line
    {
        pos2 = line.size() - 1;
    }
    std::string cat_name;
    std::string replacedword;
    //store the cat_name
    for (std::size_t i = 0; i < pos2 - pos1 - 1; i++)
    {
        cat_name.push_back(line[pos1 + i + 1]);
    }
    if (checkname(cat_name) > 0)
    { // cat_name is an int
        replacedword = chooseInt(cat_name, usedWords);
    }
    else
    { // cat_name is only characters
        replacedword = choose_word(cat_name, cat_array);
    }
    line.replace(pos1, pos2 - pos1, replacedword);
    usedWords.push_back(replacedword); // add the used word
}

//make the story according to the given story template and cat_array
void makeStory(std::vector<std::string> &story_template, std::map<std::string, std::set<std::string> > cat_array)
{
    std::vector<std::string> usedWords;
    for (size_t i = 0; i < story_template.size(); i++)
    {
        while (Checkunderline(story_template[i]) > 0) // check if exit _
        {
            doreplace(story_template[i], cat_array, usedWords); //replace the word after first _ in the line
        }
    }
}

//for test
int main()
{
    std::string line = "Once upon a time, there was a _animal. This _1 lived in a very";
    line.push_back('\n');
    std::vector<std::string> story_template;
    story_template.push_back(line);
    line = "_adjective _place. One day, it left its _2 _2 and met a _animal.";
    line.push_back('\n');
    story_template.push_back(line);

    std::map<std::string, std::set<std::string> > test_cat_array;
    std::set<std::string> test_words;
    test_words.insert("dragon");
    test_words.insert("walrus");
    test_cat_array["animal"] = test_words;
    test_words.clear();
    test_words.insert("cave");
    test_cat_array["place"] = test_words;
    test_words.clear();
    test_words.insert("peculiar");
    test_words.insert("scary");
    test_words.insert("peaceful");
    test_cat_array["adjective"] = test_words;

    makeStory(story_template, test_cat_array);
    print_story(story_template);

    return 0;
}
