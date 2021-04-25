#include "Converter.hpp"

#include "ACLIB.hpp"

#include <sstream>
#include <sys/stat.h>

namespace ACLIB
{
    std::string Converter::getKeyFromFolderName(const std::string& folder)
    {
        size_t i, name_len = folder.length();
        Sint32 key1 = 0, key2 = 0, key3 = 0;
        Sint32 key4 = 0x1683;
        Sint32 key5 = 0x42;
        Sint32 key6 = 0x65;
        Sint32 key7 = 0xab, key8 = 0xab;

        // 1
        for(i = 0; i < name_len; ++i)
            key1 += folder[i];

        // 2
        for(i = 0; i < name_len - 1; i += 2)
        {
            key2 *= folder[i];
            key2 -= folder[i + 1];
        }

        // 3
        for(i = 1; i < name_len - 3; i += 3)
        {
            key3 *= folder[i];
            key3 /= folder[i + 1] + 0x1b;
            key3 += -0x1b - folder[i - 1];
        }

        // 4
        for(i = 1; i < name_len; ++i)
            key4 -= folder[i];

        // 5
        for(i = 1; i < name_len - 4; i += 4)
        {
            key5 = (folder[i - 1] + 0xf) * (folder[i] + 0xf) * key5 + 0x16;
        }

        // 6
        for(i = 0; i < name_len - 2; i += 2)
            key6 -= folder[i];

        // 7
        for(i = 0; i < name_len - 2; i += 2)
            key7 %= folder[i];

        // 8
        for(i = 0; i < name_len - 1; ++i)
        {
            key8 = key8 / folder[i];
            key8 += folder[i + 1];
        }

        std::stringstream ss;
        ss << (key1 & 0xff) << '-';
        ss << (key2 & 0xff) << '-';
        ss << (key3 & 0xff) << '-';
        ss << (key4 & 0xff) << '-';
        ss << (key5 & 0xff) << '-';
        ss << (key6 & 0xff) << '-';
        ss << (key7 & 0xff) << '-';
        ss << (key8 & 0xff);

        return ss.str();
    }

    std::map<std::string, std::vector<char>> Converter::decryptACD(const std::string& file_name)
    {
        std::map<std::string, std::vector<char>> files;

        // Get plain folder and file name
        size_t      last_slash_index = file_name.rfind('/');
        std::string car_name         = file_name.substr(last_slash_index + 1);
        std::string folder_name      = file_name.substr(0, last_slash_index);
        folder_name                  = folder_name.substr(folder_name.rfind('/') + 1);

        // Check if file exists
        std::ifstream handle(file_name, std::ios::binary | std::ios::ate);
        if(!handle.good())
            return files;

        // Compute key
        std::string key     = Converter::getKeyFromFolderName(folder_name);
        size_t      key_len = key.length();

        // Create buffer
        std::streamsize buffer_len = handle.tellg();
        handle.seekg(0, std::ios::beg);
        std::vector<char> buffer(buffer_len);
        if(handle.read(buffer.data(), buffer_len))
        {
            handle.close();

            size_t i, index = 0;
            char*  data = buffer.data();
            if(data[0] < 0)
                index = 8;

            while(index < buffer_len)
            {
                Uint32 packed_file_name_len = *reinterpret_cast<Uint32*>(data + index);
                index += 4;

                std::string packed_file_name(data + index, data + index + packed_file_name_len);
                index += packed_file_name_len;

                Uint32 packed_file_len = *reinterpret_cast<Uint32*>(data + index);
                index += 4;

                std::vector<char> packed_file_content(packed_file_len + 1);
                for(i = 0; i < packed_file_len; ++i)
                {
                    packed_file_content[i] = data[index + i * 4] - key[i % key_len];
                }
                packed_file_content[packed_file_len] = '\0';
                index += packed_file_len * 4;

                files[packed_file_name] = packed_file_content;
            }
        }
        return files;
    }

    bool Converter::unpackACD(const std::string& file_name, const std::string& destination)
    {
        auto files = Converter::decryptACD(file_name);

        // Nothing to unpack
        if(files.empty())
            return true;

        struct stat info;
        const char* path = destination.c_str();
        stat(path, &info);

        // Directory does not exists
        if(!(info.st_mode & S_IFDIR))
            return false;

        for(auto it = files.begin(); it != files.end(); ++it)
        {
            // Check if file exists
            std::ofstream handle(destination + "/" + it->first, std::ios::out);
            handle.write(it->second.data(), it->second.size());
            handle.close();
        }
        return true;
    }
}  // namespace ACLIB
