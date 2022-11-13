rand_story: rand_story.cpp rand_story.hpp
  g++ -o rand_story -pedantic -std=gnu++11 -Wall -Werror rand_story.cpp
.PHONY: clean
clean: rm -f rand_story