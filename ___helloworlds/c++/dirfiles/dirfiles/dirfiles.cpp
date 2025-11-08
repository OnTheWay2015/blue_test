
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

 


#include <comdef.h>
std::string wstring2string(std::wstring wstr)
{
    std::string result;
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    if (len <= 0)return result;
    char* buffer = new char[len + 1];
    if (buffer == NULL)return result;
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    result.append(buffer);
    delete[] buffer;
    return result;



}

void TraverseDirectory(const std::wstring& path, std::vector< std::vector<std::wstring>>& fileList) {
    WIN32_FIND_DATAW findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // 构造搜索路径
    std::wstring searchPath = path + L"\\*.js";

    hFind = FindFirstFileW(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        std::wcerr << L"无法打开目录: " << path << std::endl;
        return;
    }

    do {
        // 忽略当前目录和上级目录
        if (wcscmp(findData.cFileName, L".") == 0 ||
            wcscmp(findData.cFileName, L"..") == 0) {
            continue;
        }

        std::wstring fullPath = path + L"/" + findData.cFileName;

        // 如果是目录则递归遍历
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            TraverseDirectory(fullPath, fileList);
        }
        else {
            fileList.push_back({ fullPath,path+L"/"});
            std::wcout << L"找到文件: " << fullPath << std::endl;
        }
    } while (FindNextFileW(hFind, &findData) != 0);

    FindClose(hFind);
}

bool WriteToFile(const std::wstring& filePath, const std::string& content) {
    HANDLE hFile = CreateFileW(
        filePath.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"无法创建文件: " << filePath << std::endl;
        return false;
    }

    DWORD bytesWritten;
    BOOL result = WriteFile(
        hFile,
        content.c_str(),
        static_cast<DWORD>(content.size()),
        &bytesWritten,
        NULL
    );

    CloseHandle(hFile);
    return result && (bytesWritten == content.size());
}

std::string ReadFromFile(const std::wstring& filePath) {
    HANDLE hFile = CreateFileW(
        filePath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"无法打开文件: " << filePath << std::endl;
        return "";
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return "";
    }

    std::string content(fileSize, '\0');
    DWORD bytesRead;
    BOOL result = ReadFile(
        hFile,
        &content[0],
        fileSize,
        &bytesRead,
        NULL
    );

    CloseHandle(hFile);
    return result ? content : "";
}

//int main() {
//    std::wstring filePath = L"test.txt";
//    std::string content = "这是测试内容\n第二行内容";
//
//    // 写入文件
//    if (WriteToFile(filePath, content)) {
//        std::wcout << L"文件写入成功: " << filePath << std::endl;
//    }
//
//    // 读取文件
//    std::string readContent = ReadFromFile(filePath);
//    if (!readContent.empty()) {
//        std::cout << "文件内容:\n" << readContent << std::endl;
//    }
//
//    return 0;
//}


std::string proc(std::wstring filePath, std::wstring path)
{

    HANDLE hFile = CreateFileW(
        filePath.c_str(),
        GENERIC_READ| GENERIC_WRITE, //读或写
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );



    if (hFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"无法打开文件: " << filePath << std::endl;
        return "";
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return "";
    }

    std::string content(fileSize, '\0');
    DWORD bytesRead;
    BOOL result = ReadFile(
        hFile,
        &content[0],
        fileSize,
        &bytesRead,
        NULL
    );
    auto wfname = filePath.substr(path.length());
 


    auto fname = wstring2string(wfname);



    std::string k = "\n\r//# sourceMappingURL=" + fname + ".map";
    ////# sourceMappingURL=403-DQeQv4q1.js.map
    content = content + k;

    /*
    //LONG offset = 0; // 从文件开头开始的偏移量
    //DWORD moveMethod = FILE_BEGIN; // 移动方法：从文件开头计算偏移量
    DWORD pos = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

    //if (!SetFilePointer(hFile, offset, NULL, moveMethod)) {
    if (!SetFilePointer(hFile, -pos, NULL, FILE_CURRENT)) {
            // 错误处理
        GetLastError();
        return "SetFilePointer error！";
    }
    */

    // 将指针从当前位置移动到文件开头(偏移量0)
    DWORD dwPtr = SetFilePointer(
        hFile,
        0,
        NULL,
        FILE_BEGIN
    );
    DWORD pos = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);



    DWORD bytesWritten;
    result = WriteFile(
        hFile,
        content.c_str(),
        static_cast<DWORD>(content.size()),
        &bytesWritten,
        NULL
    );

    if (bytesWritten <= 0) {
        std::wcerr << L"无法写入文件: " << filePath << std::endl;
    }


    CloseHandle(hFile);
}
void processfiles(std::vector< std::vector<std::wstring>> files )
{
    for (auto fns : files)
    {
        proc(fns[0],fns[1]);
    }
}

int main() {
    std::vector< std::vector<std::wstring>>files;
    //std::wstring directory = L"C:\\Test"; // 修改为要遍历的目录

    std::wstring directory = L"D:/git_work/yudao-mall-uniapp-modify/unpackage/dist/build/web/assets";


    TraverseDirectory(directory, files);

    processfiles(files);


    std::wcout << L"\n共找到 " << files.size() << L" 个文件" << std::endl;
    return 0;
}
