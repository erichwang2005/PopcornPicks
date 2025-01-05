
#include <iostream>
#include "graph_recommender.cpp"
#include "hash_recommender.cpp"
#include "hash_map.cpp"
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;
#include "json.hpp"
using json = nlohmann::json;
#include "cpp-httplib-master/httplib.h"

vector<Movie> createMoviesFromAPI(string filename) {
    vector<Movie> movies;
    ifstream file(filename);
    string line;
    const string delimiter = " ::: ";

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return movies;
    }

    while (getline(file, line)) {
        vector<string> fields;
        int start = 0;
        int end = line.find(delimiter);

        while (end != string::npos) {
            fields.push_back(line.substr(start, end - start));
            start = end + delimiter.length();
            end = line.find(delimiter, start);
        }
        fields.push_back(line.substr(start));

        string title = fields[1];
        string genre = fields[2];
        string description = fields[3];

        Movie newMovie(title, genre, description);
        movies.push_back(newMovie);
    }

    file.close();
    return movies;
}

void handleMovies(const httplib::Request& req, httplib::Response& res, vector<Movie>& movies)
{
    string result = "[";
    for(auto& movie : movies)
    {
            result += "{\"title\":\"" + movie.getName() + "\","
                    + "\"genre\":\"" + movie.getGenre() + "\","
                    + "\"description\":\"" + movie.getDescription() + "\"},";
    }

    if(!movies.empty())
    {
        result.pop_back(); //removes trailing commas
    }

    result += "]";

    // Add CORS headers
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");

    res.set_content(result, "application/json");


}

vector<string> parseGenresFromRequest(const string& body) {
    vector<string> genres;

    try {
        // Log the body to check if it's being received properly
        std::cout << "Received body: " << body << std::endl;

        // Parse the JSON string into a JSON object
        json requestJson = json::parse(body);

        // Log the parsed JSON for debugging
        std::cout << "Parsed JSON: " << requestJson.dump() << std::endl;

        // Extract the genres field if it exists
        if (requestJson.contains("genres")) {
            // Ensure "genres" is an array
            if (requestJson["genres"].is_array()) {
                for (const auto& genre : requestJson["genres"]) {
                    if (genre.is_string()) {
                        genres.push_back(genre.get<string>());
                    }
                }
            } else {
                std::cerr << "Error: 'genres' is not an array in the request body." << std::endl;
            }
        } else {
            std::cerr << "Error: 'genres' field not found in the request body." << std::endl;
        }
    } catch (const json::parse_error& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        std::cerr << "Received body: " << body << std::endl;  // Log the body for debugging
    }

    return genres;
}

void handleHashRecommendation(const httplib::Request& req, httplib::Response& res, const vector<Movie>& movies) {
    cout << "Received request body: " << req.body << endl;

    auto start = high_resolution_clock::now();

    // Parse the input genres from the request body (using nlohmann::json)
    vector<string> preferredGenres = parseGenresFromRequest(req.body);

    if (preferredGenres.empty()) {
        json response = {
            {"recommendations", json::array()},
            {"message", "No genres received."},
            {"time_taken", 0}
        };
        res.set_content(response.dump(), "application/json");
        return;
    }

    cout << "Received genres for hash recommendation: ";
    for (const auto& genre : preferredGenres) {
        cout << genre << " ";
    }
    cout << endl;

    // Hash-based recommendation logic
    vector<string> recommendedMovieTitles = useHashRecommender(movies, preferredGenres);

    json recommendations = json::array();
    if (recommendedMovieTitles.empty()) {
        recommendations = json::array();
    } else {
        for (const auto& title : recommendedMovieTitles) {
            auto it = find_if(movies.begin(), movies.end(), [&title](const Movie& movie) {
                return movie.getName() == title;
            });

            if (it != movies.end()) {
                const Movie& movie = *it;
                recommendations.push_back({
                    {"title", movie.getName()},
                    {"genre", movie.getGenre()},
                    {"description", movie.getDescription()}
                });
            }
        }
    }

    json response = {
        {"recommendations", recommendations},
        {"message", recommendedMovieTitles.empty() ? "No recommendations found" : "Recommendations found"},
        {"time_taken", duration_cast<microseconds>(high_resolution_clock::now() - start).count()}
    };

    res.set_content(response.dump(), "application/json");
}

void handleGraphRecommendation(const httplib::Request& req, httplib::Response& res, const vector<Movie>& movies) {
    cout << "Received request body: " << req.body << endl;

    auto start = high_resolution_clock::now();

    // Parse the input genres from the request body (using nlohmann::json)
    vector<string> preferredGenres = parseGenresFromRequest(req.body);

    if (preferredGenres.empty()) {
        json response = {
            {"recommendations", json::array()},
            {"message", "No genres received."},
            {"time_taken", 0}
        };
        res.set_content(response.dump(), "application/json");
        return;
    }

    cout << "Received genres for hash recommendation: ";
    for (const auto& genre : preferredGenres) {
        cout << genre << " ";
    }
    cout << endl;

    // Hash-based recommendation logic
    vector<string> recommendedMovieTitles = useGraphRecommender(movies, preferredGenres);

    json recommendations = json::array();
    if (recommendedMovieTitles.empty()) {
        recommendations = json::array();
    } else {
        for (const auto& title : recommendedMovieTitles) {
            auto it = find_if(movies.begin(), movies.end(), [&title](const Movie& movie) {
                return movie.getName() == title;
            });

            if (it != movies.end()) {
                const Movie& movie = *it;
                recommendations.push_back({
                    {"title", movie.getName()},
                    {"genre", movie.getGenre()},
                    {"description", movie.getDescription()}
                });
            }
        }
    }

    json response = {
        {"recommendations", recommendations},
        {"message", recommendedMovieTitles.empty() ? "No recommendations found" : "Recommendations found"},
        {"time_taken", duration_cast<microseconds>(high_resolution_clock::now() - start).count()}
    };

    res.set_content(response.dump(), "application/json");
}


void handleOptions(const httplib::Request& req, httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
    res.set_content("", "text/plain");
}

int main() {
    string csvFilePath = "movie_genre_description.txt";
    vector<Movie> movies = createMoviesFromAPI(csvFilePath);

    httplib::Server svr;

    // Serve static files (HTML, JS, CSS)
    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        std::ifstream file("index.html");
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            res.set_content(content, "text/html");
        } else {
            res.status = 404;
        }
    });

    svr.Get("/scripts.js", [](const httplib::Request& req, httplib::Response& res) {
        std::ifstream file("scripts.js");
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            res.set_content(content, "application/javascript");
        } else {
            res.status = 404;
        }
    });

    svr.Get("/styles.css", [](const httplib::Request& req, httplib::Response& res) {
        std::ifstream file("styles.css");
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            res.set_content(content, "text/css");
        } else {
            res.status = 404;
        }
    });

    svr.Get("/movies", [&movies](const httplib::Request& req, httplib::Response& res) {
        handleMovies(req, res, movies);
    });

    svr.Options("/movies", handleOptions);
    svr.Options("/recommend/graph", handleOptions);
    svr.Options("/recommend/hash", handleOptions);

    svr.Post("/recommend/graph", [&movies](const httplib::Request& req, httplib::Response& res) {
        handleGraphRecommendation(req, res, movies);
    });

    svr.Post("/recommend/hash", [&movies](const httplib::Request& req, httplib::Response& res) {
        handleHashRecommendation(req, res, movies);
    });

    // Start the server
    cout << "Starting server on port 8081..." << endl;
    svr.listen("localhost", 8081);
    return 0;
}