#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "movie.cpp"
using namespace std;

class MoviesGraph {
private:
    unordered_map<string, vector<string>> UserToGenreAdjList;
    unordered_map<string, vector<Movie>> GenreToMovieAdjList;

public:
    void addUser(string username) {
        if (UserToGenreAdjList.find(username) == UserToGenreAdjList.end())
            UserToGenreAdjList[username] = vector<string>();
        else cout << "User " << username << " already exists." << endl;
    }

    void addGenreToUser(string user, string genre) {
        if (UserToGenreAdjList.find(user) != UserToGenreAdjList.end())
            UserToGenreAdjList[user].push_back(genre);
        else cout << "User " << user << " does not exist." << endl;
    }

    void addMovieToGenre(string genre, Movie newMovie) {
        GenreToMovieAdjList[genre].push_back(newMovie);
    }

    void addPreferences(string username, vector<string> preferredGenres) {
        for (auto prefGenre : preferredGenres)
            addGenreToUser(username, prefGenre);
    }

    vector<string> getCommonPrefs() {
        auto i = UserToGenreAdjList.begin();
        unordered_set<string> commonPrefs(i->second.begin(), i->second.end());

        for (++i; i != UserToGenreAdjList.end(); i++) {
            unordered_set<string> tempSet(i->second.begin(), i->second.end());
            unordered_set<string> newCommonPrefs;
            for (const auto& genre : commonPrefs)
                if (tempSet.find(genre) != tempSet.end())
                    newCommonPrefs.insert(genre);
            commonPrefs = std::move(newCommonPrefs);
            if (commonPrefs.empty()) break;
        }
        return vector<string>(commonPrefs.begin(), commonPrefs.end());
    }

    set<Movie> getCommonMovies(vector<string> commonGenres) {
        set<Movie> commonMovies;
        for (auto genre : commonGenres)
            for (auto movie : GenreToMovieAdjList[genre])
                commonMovies.insert(movie);
        for (auto movie : commonMovies) cout << movie.getName() << endl;
        return commonMovies;
    }
};
