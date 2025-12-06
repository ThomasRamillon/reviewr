#include "json.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <string>


JsonHandler::JsonHandler() {
    // Constructor implementation (e.g., load JSON data)
    // load data.json if exists else initializes one
    this->data = nlohmann::json();
    std::ifstream file("data.json");

    if (file.is_open()) {
        file >> this->data; // assuming jsonData is a member of type nlohmann::json
        file.close();
    }
    else {
        this->data = nlohmann::json::object(); // initialize as empty JSON object
        this->data["reviews"] = nlohmann::json::array();
        this->data["artists"] = nlohmann::json::array();
        this->data["albums"] = nlohmann::json::array();
        this->data["genres"] = nlohmann::json::array();
    }
}

bool JsonHandler::artist_exists(const std::string& name) {
    auto to_lower = [](const std::string &s) {
        std::string out;
        out.reserve(s.size());
        for (unsigned char c : s) out.push_back(std::tolower(c));
        return out;
    };

    std::string target = to_lower(name);
    for (const auto& artist : this->data["artists"]) {
        std::string aname = artist.value("name", std::string());
        if (to_lower(aname) == target) {
            return true;
        }
    }
    return false;
}

bool JsonHandler::album_exists(const std::string& name) {
    for (const auto& album : data["albums"]) {
        if (album["name"] == name) {
            return true;
        }
    }
    return false;
}

bool JsonHandler::genre_exists(const std::string& name) {
    for (const auto& genre : data["genres"]) {
        if (genre["name"] == name) {
            return true;
        }
    }
    return false;
}




int JsonHandler::review_album(const std::string& albumName, const std::string& artistName, const std::vector<std::string>& genres, const std::string& review) {
    nlohmann::json newAlbum;
    newAlbum["type"] = 3;
    newAlbum["name"] = albumName;
    newAlbum["artist"] = artistName;
    newAlbum["genres"] = genres;
    newAlbum["review"] = review;
    // date with yyyy-mm-dd format
    newAlbum["date"] = __DATE__;
    // unique id generation
    newAlbum["id"] = std::to_string(std::time(nullptr));

    data["reviews"].push_back(newAlbum);

    

    // Save the updated data back to the file
    std::ofstream file("data.json");
    if (file.is_open()) {
        file << data.dump(4); // pretty print with 4 spaces indentation
        file.close();
    }
    return 0;
}

int JsonHandler::add_artist(const std::string& artistName, const std::vector<std::string>& genres) {
    nlohmann::json newArtist;
    newArtist["name"] = artistName;
    newArtist["genres"] = genres;

    data["artists"].push_back(newArtist);

    // Save the updated data back to the file
    std::ofstream file("data.json");
    if (file.is_open()) {
        file << data.dump(4); // pretty print with 4 spaces indentation
        file.close();
    }
    return 0;
}

int JsonHandler::add_genre(const std::string& genreName) {
    nlohmann::json newGenre;
    newGenre["name"] = genreName;

    data["genres"].push_back(newGenre);

    // Save the updated data back to the file
    std::ofstream file("data.json");
    if (file.is_open()) {
        file << data.dump(4); // pretty print with 4 spaces indentation
        file.close();
    }
    return 0;
}