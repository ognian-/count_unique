#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

#include "nlohmann/json.hpp"

using CompactGenome = std::map<size_t, char>;
using LeafSet = std::vector<std::vector<CompactGenome>>;
using Label = std::pair<CompactGenome, LeafSet>;

static CompactGenome get_compact_genome(const nlohmann::json& json, size_t compact_genome_index) {
	CompactGenome result;
	for (auto& mutation : json["compact_genomes"][compact_genome_index]) {
		size_t position = mutation[0];
		std::string new_base = mutation[1][1].get<std::string>();
		result[position] = new_base.at(0);
	}
	return result;
}

static LeafSet get_leaf_set(const nlohmann::json& json, size_t node_index) {
	LeafSet result;
	for (auto& clade : json["nodes"][node_index][1]) {
		std::vector<CompactGenome> leafs;
		for (size_t leaf : clade) {
			leafs.push_back(get_compact_genome(json, leaf));
		}
		std::sort(leafs.begin(), leafs.end());
		result.push_back(leafs);
	}
	std::sort(result.begin(), result.end());
	return result;
}

static void add_nodes(const nlohmann::json& json, std::set<Label>& unique) {
	size_t node_index = 0;
	for (auto& node : json["nodes"]) {
		size_t compact_genome_index = node[0];
		unique.insert({get_compact_genome(json, compact_genome_index),
			get_leaf_set(json, node_index++)});
	}
}

int main(int argc, char** argv) {
	
	std::set<Label> unique;
	for (int i = 1; i < argc; ++i) {
		std::ifstream stream(argv[i]);
		nlohmann::json json;
		stream >> json;
		add_nodes(json, unique);
	}

	std::cout << "Unique nodes: " << unique.size() << "\n";
}
