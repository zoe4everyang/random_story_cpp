rand_story: rand_story.cpp rand_story.hpp
	g++ -o rand_story -std=gnu++11 -pedantic -Wall -Werror -ggdb3 rand_story.cpp
.PHONY: clean
clean: rm -f rand_story