#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <string>
#include <cstdlib>

class CFileSystem
{
public:
    static std::string GetPath(const std::string& path)
    {
        static std::string(*pathBuilder)(std::string const &) = getPathBuilder();
        return (*pathBuilder)(path);
    }

    static bool FileExists(const std::string& fName)
    {

        std::ifstream infile(fName);
        return infile.good();
    }

private:
    typedef std::string (*Builder) (const std::string& path);

    static std::string const & getRoot()
    {

        static char const * envRoot = getenv("HOME");
        static char const * givenRoot = "/home/stn/Source/GLDevdor04";
        static std::string root = (givenRoot != nullptr ? givenRoot : envRoot);
        return root;
    }

    //static std::string(*foo (std::string const &)) getPathBuilder()
    static Builder getPathBuilder()
    {

        if (getRoot() != "")
            return &CFileSystem::getPathRelativeRoot;
        else
            return &CFileSystem::getPathRelativeBinary;
    }

    static std::string getPathRelativeRoot(const std::string& path)
    {
        return getRoot() + std::string("/") + path;
    }

    static std::string getPathRelativeBinary(const std::string& path)
    {
        return "../../../" + path;
    }
};
#endif