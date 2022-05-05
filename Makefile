all:
	@g++ -std=c++17 -Wall -Wextra -Werror -pedantic -Wold-style-cast -O0 -g3 -o count_unique main.cpp
	@./count_unique final-tree-1.nh1.pb.json  final-tree-2.nh1.pb.json  final-tree-3.nh1.pb.json

clean:
	@rm -f count_unique
