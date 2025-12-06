#pragma once
#include <nlohmann/json.hpp>
#include <string>

class JsonHandler {
private:
    nlohmann::json data;

    bool artist_exists(const std::string& name);
    bool album_exists(const std::string& name);
    bool genre_exists(const std::string& name); 

    int add_album(const std::string& albumName, const std::string& artistName, const std::vector<std::string>& genres);
    int add_artist(const std::string& artistName, const std::vector<std::string>& genres);
    int add_genre(const std::string& genreName);

    int add_album_to_genre(const std::string& albumName, const std::string& genreName);
    int add_artist_to_genre(const std::string& artistName, const std::string& genreName);
public:
    JsonHandler();

    int review_album(const std::string& albumName, const std::string& artistName, const std::vector<std::string>& genres, const std::string& review);
    int review_artist(const std::string& artistName, const std::vector<std::string>& genres, const std::string& review);
    int review_genre(const std::string& genreName, const std::string& description);

};
