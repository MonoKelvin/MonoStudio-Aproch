#include <iostream>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <shlobj.h>
#include <fstream>
#include <sstream>
#pragma comment(lib, "shell32.lib")

using std::string;
using std::vector;

class cf2d_options
{
public:
    std::string src_dir;
    std::string dest_dir;
    std::string excludes_file_name;
    std::vector<std::string> suffix_filters;

private:
    std::vector<std::string> _exclude_files;
    std::vector<std::string> _files;

public:
    void copy_file_to_directory()
    {
        _files.clear();
        _exclude_files.clear();

        if (src_dir.back() == '/' || src_dir.back() == '\\')
            src_dir.pop_back();

        if (dest_dir.back() == '/' || dest_dir.back() == '\\')
            dest_dir.pop_back();

        _get_exclude_files(excludes_file_name, _exclude_files);
        _get_all_files(src_dir, suffix_filters, _exclude_files, _files);

        for (const auto& file : _files)
        {
            std::filesystem::directory_entry fs_file(file);
            string parent_path = fs_file.path().parent_path().string();

            std::replace(parent_path.begin(), parent_path.end(), '\\', '/');
            string new_int_path = parent_path.substr(src_dir.size(), parent_path.size() - src_dir.size());

            string new_dest_dir = dest_dir + new_int_path;
            _crete_directory(new_dest_dir);

            string new_file = new_dest_dir + "/" + fs_file.path().filename().string();
            _copy_file_to_directory(file, new_file);
        }
    }

    const std::vector<std::string>& get_files_to_copy() const
    {
        return _files;
    }

private:
    wchar_t* _str_2_wstr(const string& str)
    {
        size_t size = str.length();
        wchar_t* buffer = new wchar_t[size + 1];
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), size, buffer, size * sizeof(wchar_t));
        buffer[size] = 0;
        return buffer;
    }

    void _get_exclude_files(const std::string& exclude_file_name, std::vector<std::string>& exc_files)
    {
        std::ifstream file;
        file.open(exclude_file_name, std::ios_base::in);

        if (!file.is_open())
        {
            std::cout << "file can not open" << std::endl;
            return;
        }

        string s;
        while (getline(file, s))
            exc_files.emplace_back(s);
        file.close();
    }

    void _crete_directory(string path)
    {
        string::size_type pos;
        while ((pos = path.find("/")) != string::npos)
            path = path.replace(pos, 1, "\\");

        wchar_t* buffer = _str_2_wstr(path);
        SHCreateDirectoryEx(NULL, buffer, NULL);
        delete buffer;
    }

    void _copy_file_to_directory(const string& filename, const string& new_filename)
    {
        wchar_t* wc_filename = _str_2_wstr(filename);
        wchar_t* wc_new_filename = _str_2_wstr(new_filename);
        CopyFile(wc_filename, wc_new_filename, FALSE);
        delete wc_filename;
        delete wc_new_filename;
    }

    void _get_all_files(const std::string& src_path,
                        const std::vector<std::string>& suffix_filters,
                        const std::vector<std::string>& exclude_files,
                        std::vector<std::string>& files)
    {
        for (const auto& entry : std::filesystem::directory_iterator(src_path))
        {
            string file_path = entry.path().string();
            std::replace(file_path.begin(), file_path.end(), '\\', '/');
            if (entry.is_directory())
            {
                size_t i = file_path.find_last_of('/');
                if (i == std::string::npos)
                    i = file_path.find_last_of('\\');
                string new_path = src_path + "/" + file_path.substr(i + 1, file_path.size() - i);
                _get_all_files(new_path, suffix_filters, exclude_files, files);
            }
            else
            {
                const std::string ext = entry.path().extension().string();
                if (std::find(suffix_filters.cbegin(), suffix_filters.cend(), ext) == suffix_filters.cend())
                {
                    continue;
                }

                const std::string file_base_name = entry.path().filename().string();
                if (std::find(exclude_files.cbegin(), exclude_files.cend(), file_base_name) != exclude_files.cend())
                    continue;

                files.emplace_back(file_path);
            }
        }
    }
};

std::vector<std::string> split_string(const std::string& str, char tag)
{
    std::vector<std::string> li;
    std::string subStr;
    for (size_t i = 0; i < str.length(); i++)
    {
        if (tag == str[i])
        {
            if (!subStr.empty())
            {
                li.push_back(subStr);
                subStr.clear();
            }
        }
        else
        {
            subStr.push_back(str[i]);
        }
    }

    if (!subStr.empty())
    {
        li.emplace_back(subStr);
    }

    return li;
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Too few parameters" << std::endl;
        return 0;
    }

    cf2d_options cf2d_op;

    std::string src_dir = argv[1];
    std::filesystem::directory_entry fs_src_dir(src_dir);
    if (!fs_src_dir.is_directory())
    {
        std::cout << "The second parameter is not directory" << std::endl;
        return 0;
    }
    cf2d_op.src_dir = fs_src_dir.path().string();

    std::string dest_dir = argv[2];
    std::filesystem::directory_entry fs_dest_dir(dest_dir);
    if (!fs_dest_dir.is_directory())
    {
        std::cout << "The third parameter is not directory" << std::endl;
        return 0;
    }
    cf2d_op.dest_dir = fs_dest_dir.path().string();

    for (int i = 3; i < argc; ++i)
    {
        std::string option_string = argv[i];
        if (option_string.size() < 1 || option_string[0] != '-')
            continue;

        const size_t option_last_pos = option_string.find_first_of(':');
        if (option_last_pos == std::string::npos)
        {
            std::cout << "invliad option: " << "\"" << option_string << "\"" << std::endl;
            continue;
        }

        const std::string op = option_string.substr(1, option_last_pos - 1);
        const std::string parameter = option_string.substr(option_last_pos + 1, 
                                                           option_string.size() - option_last_pos - 1);
        if (op == "s") // suffix filter
        {
            cf2d_op.suffix_filters = split_string(parameter, ';');
            for (auto iter = cf2d_op.suffix_filters.begin(); iter != cf2d_op.suffix_filters.end(); )
            {
                if (iter->size() == 0)
                    iter = cf2d_op.suffix_filters.erase(iter);
                else
                {
                    if (iter->front() == '*')
                        *iter = iter->erase(0, 1);
                    if (iter->front() != '.')
                        iter->insert(iter->begin(), '.');
                    ++iter;
                }
            }
        }
        else if (op == "e")
        {
            std::filesystem::directory_entry fs_exclude_file(parameter);
            if (!fs_exclude_file.exists())
            {
                std::cout << "the excluded file not exists: " << "\"" << option_string << "\"" << std::endl;
                continue;
            }
            cf2d_op.excludes_file_name = fs_exclude_file.path().string();
        }
    }

    cf2d_op.copy_file_to_directory();

    return 0;
}