#include <iostream>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

using std::string;
using std::vector;

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

wchar_t* str_2_wstr(const string& str)
{
    size_t size = str.length();
    wchar_t* buffer = new wchar_t[size + 1];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), size, buffer, size * sizeof(wchar_t));
    buffer[size] = 0;
    return buffer;
}

void crete_directory(string path)
{
    string::size_type pos;
    while ((pos = path.find("/")) != string::npos)
        path = path.replace(pos, 1, "\\");

    wchar_t* buffer = str_2_wstr(path);
    SHCreateDirectoryEx(NULL, buffer, NULL);
    delete buffer;
}

void copy_file_to_directory(const string& filename, const string& new_filename)
{
    wchar_t* wc_filename = str_2_wstr(filename);
    wchar_t* wc_new_filename = str_2_wstr(new_filename);
    CopyFile(wc_filename, wc_new_filename, FALSE);
    delete wc_filename;
    delete wc_new_filename;
}

void get_all_files(const string& path, const std::string& format, vector<string>& files)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        string file_path = entry.path().string();
        std::replace(file_path.begin(), file_path.end(), '\\', '/');
        if (entry.is_directory())
        {
            int i = file_path.find_last_of('/');
            if (i < 0)
                i = file_path.find_last_of('\\');
            string new_path = path + "/" + file_path.substr(i + 1, file_path.size() - i);
            get_all_files(new_path, format, files);
        }
        else
        {
            if (format != entry.path().extension())
                continue;
            files.emplace_back(file_path);
        }
    }
}

int main(int argc, char* argv[])
{
    std::vector<std::string> files;
    std::string src_dir = "E:/Project/Code/MonoStudio/Aproch/Src";
    std::string dest_dir = "E:/Project/Code/MonoStudio/Aproch/Build/Include";
    std::string format = ".h";
    get_all_files(src_dir, format, files);
    for (const auto& file : files)
    {
        //std::cout << file << std::endl;
        std::filesystem::directory_entry fs_file(file);
        string parent_path = fs_file.path().parent_path().string();

        std::replace(parent_path.begin(), parent_path.end(), '\\', '/');
        string new_int_path = parent_path.substr(src_dir.size(), parent_path.size() - src_dir.size());

        string new_dest_dir = dest_dir + new_int_path;
        crete_directory(new_dest_dir);

        string new_file = new_dest_dir + "/" + fs_file.path().filename().string();
        copy_file_to_directory(file, new_file);
    }

    return 0;

    //if (argc < 4)
    //    throw "Too few parameters，example: [ CopyFileToDirectory.exe \"C:/src_dir/\" \"C:/dest_dir/\" \"*.h\" -t ]";

    //std::string src_dir = argv[1];
    //std::string dest_dir = argv[2];

    //// file filter
    //std::string file_filter = argv[3];
    //std::vector<std::string> file_filters = split_string(file_filter, ';');

    //// copy tree structure
    //bool copy_tree_struct = false;
    //if (argc > 4)
    //    copy_tree_struct = argv[4] == std::string("-t");

    //std::cout << "Hello World!\n";
}