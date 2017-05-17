#include "filehandler.h"
#include <fstream>
#include <iostream>

namespace LIMoSim
{

FileHandler::FileHandler()
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

bool FileHandler::exists(const std::string &_path)
{
    std::ifstream file;
    std::string line;
    file.open(_path);
    if(file.is_open())
    {
        file.close();
        return true;
    }
    return false;
}

void FileHandler::append(const std::string &_data, const std::string &_path)
{
    std::ofstream outfile;
    outfile.open(_path, std::ios_base::app);
    outfile << _data;
}

bool FileHandler::write(const std::string &_data, const std::string &_path)
{
    std::cout << "FileHandler::write " << _path << std::endl;

    std::ofstream outfile;
    outfile.open(_path);
    if(outfile.is_open())
    {
        outfile << _data;
        outfile.close();
        return true;
    }
    else
        std::cout << "FileHandler::write failed to open " << _path << std::endl;

    return false;
}

std::vector<std::string> FileHandler::read(const std::string &_path)
{
    std::vector<std::string> lines;

    std::ifstream file;
    std::string line;
    file.open(_path);
    if(file.is_open())
    {
        while (!file.eof())
        {
            getline(file, line);
            lines.push_back(line);
        }
        file.close();
    }
    else
        std::cout << "FileHandler::read failed to open " << _path << std::endl;

    return lines;
}

std::string FileHandler::listToString(const std::vector<std::string> &_data)
{
    std::string data;
    for(unsigned int i=0; i<_data.size(); i++)
        data += _data.at(i);
    return data;
}

}
