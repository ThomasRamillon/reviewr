#include "cli_parser.hpp"
#include <iostream>

CommandLineParser::CommandLineParser() = default;

void CommandLineParser::registerFlag(char shortForm, const std::string& longForm, bool takesValue) {
    shortFlags[shortForm] = longForm;
    if (takesValue) {
        valueFlags.insert(longForm);
    } else {
        boolFlags.insert(longForm);
    }
}

bool CommandLineParser::isFlag(const std::string& arg) const {
    return !arg.empty() && arg[0] == '-';
}

std::string CommandLineParser::getLongForm(const std::string& flag) const {
    // Remove leading dashes
    std::string cleanFlag = flag;
    if (cleanFlag[0] == '-') {
        cleanFlag = cleanFlag.substr(1);
        if (!cleanFlag.empty() && cleanFlag[0] == '-') {
            cleanFlag = cleanFlag.substr(1);
        }
    }
    
    // If it's a single character short flag, look it up
    if (cleanFlag.length() == 1 && shortFlags.count(cleanFlag[0])) {
        return shortFlags.at(cleanFlag[0]);
    }
    
    return cleanFlag;
}

bool CommandLineParser::parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (!isFlag(arg)) {
            // It's a positional argument
            positionalArgs.push_back(arg);
            continue;
        }
        
        // Handle long flags (--flag)
        if (arg[1] == '-') {
            std::string longFlag = arg.substr(2);
            
            if (valueFlags.count(longFlag)) {
                // This flag takes a value
                if (i + 1 >= argc || isFlag(argv[i + 1])) {
                    std::cerr << "Error: flag --" << longFlag << " requires a value" << std::endl;
                    return false;
                }
                flagValues[longFlag] = argv[++i];
                activeFlags.insert(longFlag);
            } else if (boolFlags.count(longFlag)) {
                activeFlags.insert(longFlag);
            } else {
                std::cerr << "Error: unknown flag --" << longFlag << std::endl;
                return false;
            }
            continue;
        }
        
        // Handle short flags (-v, -d, or combined -vd)
        for (size_t j = 1; j < arg.length(); ++j) {
            char shortFlag = arg[j];
            
            if (shortFlags.count(shortFlag) == 0) {
                std::cerr << "Error: unknown flag -" << shortFlag << std::endl;
                return false;
            }
            
            std::string longForm = shortFlags.at(shortFlag);
            
            if (valueFlags.count(longForm)) {
                // This flag takes a value
                if (i + 1 >= argc || isFlag(argv[i + 1])) {
                    std::cerr << "Error: flag -" << shortFlag << " requires a value" << std::endl;
                    return false;
                }
                flagValues[longForm] = argv[++i];
                activeFlags.insert(longForm);
            } else if (boolFlags.count(longForm)) {
                activeFlags.insert(longForm);
            }
        }
    }
    
    return true;
}

bool CommandLineParser::hasFlag(const std::string& flagName) const {
    return activeFlags.count(flagName) > 0;
}

bool CommandLineParser::hasFlag(char shortForm) const {
    if (shortFlags.count(shortForm)) {
        return activeFlags.count(shortFlags.at(shortForm)) > 0;
    }
    return false;
}

std::string CommandLineParser::getValue(const std::string& flagName) const {
    auto it = flagValues.find(flagName);
    if (it != flagValues.end()) {
        return it->second;
    }
    return "";
}

std::string CommandLineParser::getValue(char shortForm) const {
    if (shortFlags.count(shortForm)) {
        return getValue(shortFlags.at(shortForm));
    }
    return "";
}

std::unordered_set<std::string> CommandLineParser::getFlags() const {
    return activeFlags;
}

std::vector<std::string> CommandLineParser::getPositionalArgs() const {
    return positionalArgs;
}
