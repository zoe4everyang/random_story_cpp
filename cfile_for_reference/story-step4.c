#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "rand_story.h"

int main (int argc, char ** argv){
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: story-step4 (optional)-n wordFile storyFile\n");
        exit(EXIT_FAILURE); 
    }
    bool noReuse = !strcmp(argv[1], "-n"); // if same, strcmp return 0 and noReuse is 1
    if (argc == 4 && noReuse == 0) {
        fprintf(stderr, "Usage: story-step4 (optional)-n wordFile storyFile\n");
        exit(EXIT_FAILURE);
    }
    
    FILE * f1;
    FILE * f2;
    lineArray_t * cateWordPairs;
    lineArray_t * storyTemplate;
    if (!noReuse) // should reuse
    {
        if (argv[1] == NULL) {
            fprintf(stderr, "wordFile is NULL\n");
            exit(EXIT_FAILURE);
        }
        if (argv[2] == NULL) {
            fprintf(stderr, "storyFile is NULL\n");
            exit(EXIT_FAILURE);
        }
        f1 = fopen(argv[1], "r");
        if (f1 == NULL) {
            fprintf(stderr, "Can not open wordFile\n");
            exit(EXIT_FAILURE);
        }
        cateWordPairs = readLines(f1);
        if (fclose(f1) != 0) {
            freeLines(cateWordPairs);
            fprintf(stderr, "Can not close wordFile\n");
            exit(EXIT_FAILURE);
        }
        f2 = fopen(argv[2], "r");
        if (f2 == NULL) {
            fprintf(stderr, "Can not open storyFile\n");
            exit(EXIT_FAILURE);
        }
        storyTemplate = readLines(f2);
        if (fclose(f2) != 0) {
            freeLines(storyTemplate);
            fprintf(stderr, "Can not close storyFile\n");
            exit(EXIT_FAILURE);
        }
    } else { // should not reuse
        if (argv[2] == NULL) {
            fprintf(stderr, "wordFile is NULL\n");
            exit(EXIT_FAILURE);
        }
        if (argv[3] == NULL) {
            fprintf(stderr, "storyFile is NULL\n");
            exit(EXIT_FAILURE);
        }
        f1 = fopen(argv[2], "r");
        if (f1 == NULL) {
            fprintf(stderr, "Can not open wordFile\n");
            exit(EXIT_FAILURE);
        }
        cateWordPairs = readLines(f1);
        if (fclose(f1) != 0) {
            freeLines(cateWordPairs);
            fprintf(stderr, "Can not close wordFile\n");
            exit(EXIT_FAILURE);
        }
        f2 = fopen(argv[3], "r");
        if (f2 == NULL) {
            fprintf(stderr, "Can not open storyFile\n");
            exit(EXIT_FAILURE);
        }
        storyTemplate = readLines(f2);
        if (fclose(f2) != 0) {
            freeLines(storyTemplate);
            fprintf(stderr, "Can not close storyFile\n");
            exit(EXIT_FAILURE);
        }
    }


    catarray_t * cats = parseCatArr(cateWordPairs);
    lineArray_t * storyGenerate = replaceBlanksInFile(storyTemplate, cats, noReuse);

    printLines(storyGenerate);

    freeLines(storyGenerate);
    freeCateArr(cats);

    freeLines(storyTemplate);
    freeLines(cateWordPairs);

    return EXIT_SUCCESS;
}
