#ifndef LIMOSIM_FILEHANDLER_H
#define LIMOSIM_FILEHANDLER_H

#include <vector>
#include <string>

namespace LIMoSim
{

class FileHandler
{
public:
    FileHandler();

    static bool exists(const std::string &_path);
    static void append(const std::string &_data, const std::string &_path);
    static bool write(const std::string &_data, const std::string &_path);
    static std::vector<std::string> read(const std::string &_path);

    static std::string listToString(const std::vector<std::string> &_data);
};

}

#endif // LIMOSIM_FILEHANDLER_H
