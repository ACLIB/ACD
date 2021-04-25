#ifndef ACLIB_ACD_PYEVENTLOOP_HPP
#define ACLIB_ACD_PYEVENTLOOP_HPP

#include <map>
#include <string>
#include <vector>

namespace ACLIB
{
    class Converter
    {
    public:
        /**
         * @brief   Computes the key used to encrypt files, stored in a *.acd file.
         * @param   folder Name of the folder where the *.acd file is placed.
         * @return  Returns the encryption key as string. The key contains 8 key joined by
         *          dashes (i.e. "1-2-3-55-96-4-9-255").
         */
        static std::string getKeyFromFolderName(const std::string& folder);

        /**
         * @brief   Decrypts a *.acd file and stores the decrypted files and their contents into a
         *          map.
         * @param   file_name Full path where the file is stored.
         * @return  Returns a map containing the file names as key and their contents as values.
         *          Contents are stored as utf-8.
         */
        static std::map<std::string, std::vector<char>> decryptACD(const std::string& file_name);

        /**
         * @brief   Decrypts a *.acd file and write the contents to the destination path. The
         *          destination path must exist.
         * @param file_name     Full path where the *.acd file is stored.
         * @param destination   Full path where files are stored.
         */
        static bool unpackACD(const std::string& file_name, const std::string& destination);
    };
}  // namespace ACLIB

#endif  // ACLIB_ACD_PYEVENTLOOP_HPP
