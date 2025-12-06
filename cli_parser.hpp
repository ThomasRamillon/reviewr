#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_set>

class CommandLineParser {
private:
    std::map<char, std::string> shortFlags;      // Maps short flags (-v) to long form (verbose)
    std::unordered_set<std::string> boolFlags;   // Boolean flags (present or not)
    std::unordered_set<std::string> valueFlags;  // Flags that take values (--output <file>)
    
public:
    CommandLineParser();
    
    // Register a flag: short form, long form, is it a value flag?
    void registerFlag(char shortForm, const std::string& longForm, bool takesValue = false);
    
    // Parse command line arguments
    bool parse(int argc, char* argv[]);
    
    // Check if a flag is present
    bool hasFlag(const std::string& flagName) const;
    bool hasFlag(char shortForm) const;
    
    // Get flag value (for flags that take values)
    std::string getValue(const std::string& flagName) const;
    std::string getValue(char shortForm) const;
    
    // Get all set flags
    std::unordered_set<std::string> getFlags() const;
    
    // Get positional arguments (non-flag arguments)
    std::vector<std::string> getPositionalArgs() const;
    
private:
    std::unordered_set<std::string> activeFlags;  // Flags that were set
    std::map<std::string, std::string> flagValues; // Values for flags
    std::vector<std::string> positionalArgs;
    
    bool isFlag(const std::string& arg) const;
    std::string getLongForm(const std::string& flag) const;
};
