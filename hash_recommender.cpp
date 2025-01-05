
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include "hash_map.h"

using namespace std;
#include <cstdint>
vector<string> hashRecommender(popPickTable& movieTable, const unordered_set<string>& sharedGenres) {
    vector<string> recommendations;
    for (int i = 0; i < movieTable.size(); i++) {
        if (movieTable.occupied[i]) {
            string movieGenre = movieTable.table[i].value;
            if (sharedGenres.find(movieGenre) != sharedGenres.end()) {
                recommendations.push_back(movieTable.table[i].key);
            }
        }
    }
    return recommendations;
}

vector<string> useHashRecommender(const vector<Movie>& allMovies, const vector<string>& preferredGenres) {
    popPickTable movieTable(100000);
    for (const auto& movie : allMovies) {
        movieTable.insert(movie.getName(), movie.getGenre());
    }

    unordered_set<string> sharedGenres(preferredGenres.begin(), preferredGenres.end());
    return hashRecommender(movieTable, sharedGenres);
}