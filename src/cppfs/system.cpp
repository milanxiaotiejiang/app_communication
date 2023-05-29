
#include <stdlib.h>

#include <cppfs/system.h>


namespace {

    std::string obtainHomeDir() {
        return std::string(getenv("HOME"));
    }

    std::string obtainConfigDir() {
        return std::string(getenv("HOME")) + "/.config/";
    }

} // namespace


namespace cppfs {


    namespace system {


        const std::string &homeDir() {
            static const std::string dir = obtainHomeDir();

            return dir;
        }

        std::string configDir(const std::string &application) {
            static const std::string dir = obtainConfigDir();

            return dir + application;
        }


    } // namespace system


} // namespace cppfs
