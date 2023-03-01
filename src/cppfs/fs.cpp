
#include <cppfs/fs.h>

#include <sstream>
#include <iomanip>
#include <iterator>

#include <cppfs/basen.hpp>

#include <cppfs/system.h>
#include <cppfs/LoginCredentials.h>
#include <cppfs/Url.h>
#include <cppfs/FileHandle.h>
#include <cppfs/AbstractFileSystem.h>
#include <cppfs/FileIterator.h>

#include <cppfs/posix/LocalFileSystem.h>


namespace cppfs {
    namespace fs {


        std::shared_ptr<AbstractFileSystem> localFS() {
            static std::shared_ptr<LocalFileSystem> fs(new LocalFileSystem);

            return fs;
        }

        FileHandle open(const std::string &path, const LoginCredentials *credentials) {
            // Parse url
            Url url(path);

            // Determine filesystem backend from path/url

            // SSH
            if (url.protocol() == "ssh://") {
                return FileHandle();
            }

                // Local file system
            else {
                // Get local path
                std::string localPath = url.path();

                // Open local file system
                auto fs = localFS();

                // Open path
                return fs->open(localPath);
            }
        }

        std::string sha1(const std::string &str) {
            return "";
        }

        std::string base64(const std::string &str) {
            // Encode base64
            std::string base64;
            bn::encode_b64(str.begin(), str.end(), back_inserter(base64));

            // Return encoded string
            return base64;
        }

        std::string fromBase64(const std::string &base64) {
            // Decode base64
            std::string str;
            bn::decode_b64(base64.begin(), base64.end(), back_inserter(str));

            // Return decoded string
            return str;
        }

        std::string hashToString(const unsigned char *hash) {
            std::stringstream stream;
            stream << std::hex << std::setfill('0') << std::setw(2);

            for (int i = 0; i < 20; i++) {
                stream << static_cast<unsigned int>(hash[i]);
            }

            return stream.str();
        }


    } // namespace fs
} // namespace cppfs
