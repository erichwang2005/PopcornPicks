// graph_recommender.cpp
#include "graph.cpp"
#include "movie.cpp"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

MoviesGraph createGraphWithGenreToMovieEdges(const vector<Movie>& allMovies) {
    MoviesGraph popPicker;
    for (const auto& movie : allMovies) {
        popPicker.addMovieToGenre(movie.getGenre(), movie);
    }
    return popPicker;
}

vector<string> useGraphRecommender(const vector<Movie>& movies, const vector<string>& preferredGenres) {
    vector<string> recommendedMovieTitles;

    // Filter movies that match the preferred genres
    for (const auto& movie : movies) {
        // Check if the movie's genre matches any of the preferred genres
        if (find(preferredGenres.begin(), preferredGenres.end(), movie.getGenre()) != preferredGenres.end()) {
            recommendedMovieTitles.push_back(movie.getName());  // Add only the movie name (string)
        }
    }

    // Debugging: print the recommendations to console
    cout << "Recommended Movies: " << recommendedMovieTitles.size() << endl;
    for (const auto& movieTitle : recommendedMovieTitles) {
        cout << movieTitle << endl;
    }

    return recommendedMovieTitles;
}