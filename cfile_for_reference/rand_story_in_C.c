#include "rand_story.h"

/*--------------------------------------------------------------------------------
----------------------      step 1      ------------------------------------------
--------------------------------------------------------------------------------*/
// read all lines in a file or stdin
lineArray_t * readLines (FILE * fstream) {
    lineArray_t * ans = createLines(0);
    char ** tempLines = NULL;
    char * current = NULL;
    size_t sz;
    size_t i = 0;
    while (getline(&current, &sz, fstream) >= 0) {
        tempLines = realloc(ans->lines, (i + 1) * sizeof(*ans->lines)); // in case of realloc fail
        if (tempLines == NULL) {
            freeLines(ans);
            fprintf(stderr, "realloc of ans->lines in function readLines failed.\n");
            exit(EXIT_FAILURE);
        }
        ans->lines = tempLines;

        ans->lines[i] = current;
        current = NULL;
        i++;
    }
    // getline fail
    if (errno != 0) {
        freeLines(ans);
        fprintf(stderr, "getline in function readLines failed.\n");
        exit(EXIT_FAILURE);
    }
    
    tempLines = NULL;
    free(current);
    ans->length = i;
    return ans;
}
lineArray_t * createLines (size_t length) {
    // create a NULL lineArray
    lineArray_t * ans = malloc(sizeof(*ans));
    if (ans == NULL) {
        free(ans);
        fprintf(stderr, "malloc of lineArray_t in function createLines failed.\n");
        exit(EXIT_FAILURE); 
    }
    ans->lines = NULL;
    ans->length = length;
    if (ans->length != 0) {
        ans->lines = malloc(ans->length * sizeof(*ans->lines));
        if (ans->lines == NULL) {
            free(ans->lines);
            free(ans);
            fprintf(stderr, "malloc of ans->lines in function createLines failed.\n");
            exit(EXIT_FAILURE); 
        }
    }
    return ans;
}
void freeLines(lineArray_t * a) {
    for (size_t i = 0; i < a->length; i++) {
        free(a->lines[i]);
    }
    free(a->lines);
    free(a);
}
void printLines(lineArray_t * a) {
    for (size_t i = 0; i < a->length; i++) {
        fprintf(stdout, "%s", a->lines[i]);
    }
}
char * getBlank (char * blank_begin, char * blank_end) {
    char * blank = NULL;
    size_t blLen = 0;
    blLen = strlen(blank_begin) - strlen(blank_end) - 1;
    blank = realloc(blank, (blLen + 1) * sizeof(*blank)); // one more space for \0
    strncpy(blank, blank_begin + 1, blLen);
    strncpy(blank + blLen, "", 1);
    return blank;
}
// reconstruct one line: remove blank by randomly seclected word
char * reLine(char * line, const char * word, char * blank_begin, char * blank_end) {
    size_t lenf = strlen(line) - strlen(blank_begin);
    size_t length = lenf + strlen(word) + strlen(blank_end);
    char * tempLine = malloc(length * sizeof(*tempLine));
    strncpy(tempLine, line, lenf);
    strcpy(tempLine + lenf, word);
    strcpy(tempLine + lenf + strlen(word), blank_end + 1);
    free(line);
    return tempLine;
}
// replace one blank in a line
char * rpBlank (char * line, catarray_t * cats, char * blank_begin, char * blank_end, category_t * usedWord, bool noReuse) {
    char * blank = getBlank(blank_begin, blank_end);
    const char * word = randomChoose(blank, cats, usedWord); // if not find, exit automatically
    char * tempLine = reLine(line, word, blank_begin, blank_end);
    if (!isValid(blank)) { // call used word do not need to delete word but call no-used word need to
        deleteWord(cats, blank, word, noReuse); // words can only be deleted after used
    }
    free(blank);
    return tempLine;
}
// replace all blanks within a line
char * rpLine (char * line, catarray_t * cats, category_t * usedWord, bool noReuse) {
    char * tempLine = NULL;
    tempLine = strdup(line); // for replacing line and want to keep raw data not changed
    if (tempLine == NULL) {
        freeCateArr(cats);
        freeCate(usedWord);
        fprintf(stderr, "strdup in rpLine fails. \n");
        exit(EXIT_FAILURE);
    }
    
    char * blank_begin = NULL; // line delimiter
    char * blank_end = NULL;
    blank_begin = strchr(line, '_'); // find a underscore of blank
    while (blank_begin != NULL) {
        blank_end = strchr(blank_begin + 1, '_'); // find another underscore
        if (blank_end == NULL) {
            free(tempLine);
            freeCateArr(cats);
            freeCate(usedWord);
            fprintf(stderr, "there lacks of a underscore within a blank in lines");
            exit(EXIT_FAILURE);
        }

        tempLine = rpBlank(tempLine, cats, blank_begin, blank_end, usedWord, noReuse);
        if (tempLine == NULL) {
            freeCateArr(cats);
            freeCate(usedWord);
            fprintf(stderr, "rpBlank in rpLine is NULL. \n");
            exit(EXIT_FAILURE);
        }
        blank_begin = strchr(tempLine, '_');  // find another blank
    }
    return tempLine;
}
// replace all blanks in a file
lineArray_t * replaceBlanksInFile (lineArray_t * storyTemplate, catarray_t * cats, bool noReuse) {
    lineArray_t * ans = createLines(storyTemplate->length);
    category_t * usedWord = createCate(NULL, 1);
    // for each line in input file
    for (size_t i = 0; i < storyTemplate->length; i++) {   
        ans->lines[i] = rpLine(storyTemplate->lines[i], cats, usedWord, noReuse);
    }
    freeCate(usedWord);
    return ans;
}
/*--------------------------------------------------------------------------------
----------------------      step 2      ------------------------------------------
--------------------------------------------------------------------------------*/
// find the index of category in categoryArray, return -1 if not find
int findcate (char * cate, catarray_t * ans) {
    if (ans == NULL) {
        return -1; // not find
    }
    for (size_t j = 0; j < ans->n; j++) {
        if (strcmp(cate, ans->arr[j].name) == 0) {
            return j; // find index
        }
    }
    return -1; // not find
}
category_t * createCate(category_t * cat, size_t length) {
    cat = realloc(cat, length * sizeof(*cat));
    if (cat == NULL) {
        free(cat);
        fprintf(stderr, "malloc of cat in function createCate failed.\n");
        exit(EXIT_FAILURE); 
    }
    
    cat[length - 1].name = NULL;
    cat[length - 1].n_words = 0;
    cat[length - 1].words = NULL;
    return cat;
}
void freeCate(category_t * cat) {
    // since not alloc memory for name in createCate, do not free name at here
    for (size_t i = 0; i < cat->n_words; i++) { // when n_words == 0, jump
        free(cat->words[i]);
    }
    if (cat->n_words > 0) { // when n_words == 0, jump
       free(cat->words);
    }
    free(cat);
}
void addWord(category_t * cat, const char * word) {
    cat->n_words++;
    cat->words = realloc(cat->words, cat->n_words * sizeof(* cat->words));
    cat->words[cat->n_words - 1] = strdup(word);
}
// store word in correspoding category, within a categoryArray
void storeWords(catarray_t * ans, char * cateName, char * word) {
    int index = findcate(cateName, ans);
    if (index < 0) { // category not exist
        ans->n++;
        ans->arr = createCate(ans->arr, ans->n);
        ans->arr[ans->n - 1].name = strdup(cateName);
        addWord(&ans->arr[ans->n - 1], word);
    } else { // cat exist
        addWord(&ans->arr[index],word);
    }
}
// parse one line to get one category:word pair
void parseLine(char * lines, catarray_t * ans) {
    char * cate = NULL;
    char * word = NULL;
    cate = strtok(lines, ":"); // cate: any characters, except ":"
    if (cate == NULL) { // when strtok does not find ":", it will return NULL
        fprintf(stderr, "There is one line without colon, or the file is not a wordFile.\n");
        exit(EXIT_FAILURE);
    }
    word = strtok(NULL, "\n"); // truncate word before \n
    if (strchr(word, ':') != NULL) {
        fprintf(stderr, "There is one line with more than one colons.\n");
        exit(EXIT_FAILURE);
    }
    
    storeWords(ans, cate, word);
}
// parse lines in a wordFile to get category:word pairs and construct a categoryArray
catarray_t * parseCatArr(lineArray_t * inputLines) {
    catarray_t * ans = malloc(sizeof(*ans));
    ans->n = 0; // do initialization when creating
    ans->arr = NULL;
    for (size_t i = 0; i < inputLines->length; i++) {
        parseLine(inputLines->lines[i], ans); // parse every line in wordsFile
    }
    return ans;
}
void freeCateArr(catarray_t * cat) {
    for (size_t i = 0; i < cat->n; i++) {
        for (size_t j = 0; j < cat->arr[i].n_words; j++) {
            free(cat->arr[i].words[j]);
        }
        free(cat->arr[i].words);
        free(cat->arr[i].name);
    }
    // interesting: for (size_t i = cat->n - 1; i >= 0; i--) {
    free(cat->arr);
    free(cat);
}
/*--------------------------------------------------------------------------------
----------------------      step 3      ------------------------------------------
--------------------------------------------------------------------------------*/
// check whether a string is all numbers
int isAllDigit (char * str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) { 
            return 0; // when one char is not a digit
        }
    }
    return 1;
}
// randomly choose word in category or call used word. both record the newly used word
const char * randomChoose (char * category, catarray_t * cats, category_t * usedWord) {
    if (cats == NULL) {
        return chooseWord(category, cats); // return "cat" in advance for NULL
    }
    const char * ans = NULL;
    int index = findcate(category, cats);

    // check whether "cate" in blank is a valid integer
    int numeric = isAllDigit(category);
    int backLen = (numeric)? atoi(category) : 0; // atoi(3.14) = 3; need to verify whether they are all digits
    bool validInt = numeric && backLen > 0;
    if (validInt && (index >= 0)) {
        fprintf(stderr, "There is a category that is an integer called for replacing.\n");
        exit(EXIT_FAILURE);
    }
    if (validInt) { 
        if (backLen > usedWord->n_words) {
            fprintf(stderr, "Invalid call usedWord, out of range. the number of used word is less than backLen.\n");
            exit(EXIT_FAILURE);
        }
        // return word used
        ans = usedWord->words[usedWord->n_words - backLen];
        addWord(usedWord, ans);
        return ans; // end function in advance
    }
    // randomly choose word from category
    if (index < 0) {
        fprintf(stderr, "cannot find the category.\n");
        exit(EXIT_FAILURE);
    } else {
        if (cats->arr[index].n_words == 0) {
            fprintf(stderr, "Category %s has no words when -n is given and it is called again.\n", cats->arr[index].name);
            exit(EXIT_FAILURE);
        }
        
        ans = chooseWord(category, cats);
        addWord(usedWord, ans);
    }

    if (ans == NULL) { // ans is not changed
        fprintf(stderr, "cannot get a name from cats, failed.\n");
        exit(EXIT_FAILURE);
    }
    return ans;
}
// check whether a category is a valid integer
bool isValid (char * category) {
    int numeric = isAllDigit(category);
    int backLen = (numeric)? atoi(category) : 0; // atoi(3.14) = 3; need to verify whether they are all digits
    return numeric && backLen > 0;
}
/*--------------------------------------------------------------------------------
----------------------      step 4      ------------------------------------------
--------------------------------------------------------------------------------*/
// find the index of word in a category, return -1 if not find
int findWord (category_t * cat, const char * word) {
    for (size_t i = 0; i < cat->n_words; i++) {
        if(!strcmp(cat->words[i], word) ) { 
            return i; // find the word and return its index
        }
    }
    return -1;
}
// delete word after used when -n is given
void deleteWord(catarray_t * cats, char * cate, const char * word, bool noReuse) {
    if (!noReuse) { // -n is not given, noReuse is 0
        return; // do nothing
    }
    // word is in the category, in cats->arr[index].words
    int index = findcate(cate, cats);
    int wordIndex = findWord(&cats->arr[index], word);
    if (wordIndex == -1) {
        fprintf(stderr, "Word %s is not found in category %s when deleting.\n", word, cats->arr[index].name);
        exit(EXIT_FAILURE);
    }

    free(cats->arr[index].words[wordIndex]);
    for (size_t i = wordIndex; i < cats->arr[index].n_words - 1; i++) {
        cats->arr[index].words[i] = cats->arr[index].words[i + 1]; // pop words up
    }
    cats->arr[index].words[cats->arr[index].n_words - 1] = NULL;
    cats->arr[index].n_words--; // stop the function to access the final NULL "word"
}

