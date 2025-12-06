#include <bits/stdc++.h>
#include "json.hpp"
#include "cli_parser.hpp"
#include <cstdlib>
#include <fstream>
using namespace std;

// Enter editor mode: opens $EDITOR with a temporary buffer file
// Returns the edited content, or empty string if cancelled
string editorMode() {
    // Create temporary file path
    const char* tmpdir = getenv("TMPDIR");
    if (!tmpdir) tmpdir = "/tmp";
    string bufferFile = string(tmpdir) + "/reviewr_buffer_XXXXXX";
    
    // Create temporary file
    int fd = mkstemp(&bufferFile[0]);
    if (fd == -1) {
        cerr << "Error: could not create temporary file" << endl;
        return "";
    }
    close(fd);
    
    // Get the editor from environment, default to 'vim'
    const char* editor = getenv("EDITOR");
    if (!editor) editor = "vim";
    
    cout << "Opening " << editor << " to edit your review..." << endl;
    
    // Launch editor
    string command = string(editor) + " " + bufferFile;
    int result = system(command.c_str());
    
    if (result != 0) {
        cout << "Editor closed with error or was cancelled." << endl;
        remove(bufferFile.c_str());
        return "";
    }
    
    // Read the file content
    ifstream file(bufferFile);
    if (!file.is_open()) {
        cerr << "Error: could not read buffer file" << endl;
        remove(bufferFile.c_str());
        return "";
    }
    
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    string content = buffer.str();
    
    // Clean up temporary file
    remove(bufferFile.c_str());
    
    // Trim trailing newline if present
    if (!content.empty() && content.back() == '\n') {
        content.pop_back();
    }
    
    return content;
}

int main(int argc, char* argv[]) {
    // Initialize the command-line parser
    CommandLineParser parser;
    parser.registerFlag('v', "verbose", false);   // Boolean flag
    parser.registerFlag('d', "debug", false);     // Boolean flag
    parser.registerFlag('h', "help", false);      // Boolean flag
    parser.registerFlag('a', "album", true);     // Flag with value
    parser.registerFlag('g', "genre", true);     // Flag with value
    parser.registerFlag('A', "artist", true);     // Flag with value
    parser.registerFlag('l', "list", true);     // Flag with value

    parser.parse(argc, argv);

    if (argc < 2) {
        cerr << "Insufficient arguments provided." << endl;
        return 1;
    }
    
    JsonHandler json = JsonHandler();


    if (parser.hasFlag("a") || parser.hasFlag("album")) {
        // when this command is typed, enter text editor mode

        // get album name
        string albumName = parser.getValue("album");
        if (albumName.empty()) {
            albumName = parser.getValue('a');
        }
        while (albumName.empty()) {
            cout << "Album? " << flush;
            getline(cin, albumName);
        }

        // get artist name 
        // TODO multiple artists
        string artist_name = parser.getValue("artist"); 
        if (artist_name.empty()) {
            artist_name = parser.getValue('A');
        }
        while (artist_name.empty()) {
            cout << "Artist? " << flush;
            getline(cin, artist_name);
        }


        // get genres associated
        vector<string> genres;
        string genre_input = parser.getValue("genre");
        if (genre_input.empty()) {
            genre_input = parser.getValue('g');
        }   
        while (genre_input != "-") {
            cout << "Genres (`-` to stop) ? " << flush;
            getline(cin, genre_input);
            if (!genre_input.empty() && genre_input != "-") {
                genres.push_back(genre_input);
            }
        }

        
        string review = editorMode();
        if (review.empty()) {
            // Empty review (editor closed without saving)
            cout << "Review cancelled or empty." << endl;
            return 1;
        }
        
        if (json.add_album(albumName, artist_name, genres, review)) {
            cout << "Something went wrong while adding the album." << endl;
        }
        else {
            cout << "Album added successfully." << endl;
            return 0;
        }
    } 

    if (parser.hasFlag("A") || parser.hasFlag("artist")) {
        string artist_name = parser.getValue("artist"); 
        if (artist_name.empty()) {
            artist_name = parser.getValue('A');
        }
        while (artist_name.empty()) {
            cout << "Artist? " << flush;
            getline(cin, artist_name);
        }


        // get genres associated
        vector<string> genres;
        string genre_input = parser.getValue("genre");
        if (genre_input.empty()) {
            genre_input = parser.getValue('g');
        }   
        while (genre_input != "-") {
            cout << "Genres (`-` to stop) ? " << flush;
            getline(cin, genre_input);
            if (!genre_input.empty() && genre_input != "-") {
                genres.push_back(genre_input);
            }
        }

        
        string review = editorMode();
        if (review.empty()) {
            // Empty review (editor closed without saving)
            cout << "Review cancelled or empty.";
            cout.flush();
            return 1;
        }

        if (json.add_artist(artist_name, genres, review)) {
            cout << "Something went wrong while adding the artist." << endl;
        }
        else {
            cout << "Artist added successfully." << endl;
        return 0;
        }
    }
    

    return 0;
}