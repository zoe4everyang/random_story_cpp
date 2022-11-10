#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "provided.h"
#include "ctype.h" // isdigit
#include "stdbool.h" // bool
#include <errno.h> // errno
typedef struct _lineArray_t{
    char ** lines;
    size_t length;
}lineArray_t;

// step 1
lineArray_t * readLines (FILE * fstream);
lineArray_t * createLines (size_t length);
void freeLines(lineArray_t * a); 
void printLines(lineArray_t * a);
char * getBlank (char * blank_begin, char * blank_end);
char * reLine(char * line, const char * word, char * blank_begin, char * blank_end);
char * rpBlank (char * line, catarray_t * cats, char * blank_begin, char * blank_end, category_t * usedWord, bool noReuse);
char * rpLine (char * line, catarray_t * cats, category_t * usedWord, bool noReuse);
lineArray_t * replaceBlanksInFile (lineArray_t * storyTemplate, catarray_t * cats, bool noReuse); 

// step 2
int findcate (char * cate, catarray_t * ans);
category_t * createCate(category_t * cat, size_t index);
void freeCate(category_t * cat);
void addWord(category_t * cat, const char * word);
void storeWords(catarray_t * ans, char * cate, char * word);
void parseLine(char * lines, catarray_t * ans);
catarray_t * parseCatArr(lineArray_t * lines);
void freeCateArr(catarray_t * cat);

// step3
int isAllDigit (char * str);
const char * randomChoose (char * category, catarray_t * cats, category_t * usedWord);
bool isValid (char * category);
// step4 

// modify func: replaceBlanksInFile, rpLine
int findWord (category_t * cat, const char * word);
void deleteWord(catarray_t * cats, char * cate, const char * word, bool noReuse);
#endif
