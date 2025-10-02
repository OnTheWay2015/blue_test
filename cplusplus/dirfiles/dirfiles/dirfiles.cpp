
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

    // ��������·��
    std::wstring searchPath = path + L"\\*.js";

    hFind = FindFirstFileW(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        std::wcerr << L"�޷���Ŀ¼: " << path << std::endl;
        return;
    }

    do {
        // ���Ե�ǰĿ¼���ϼ�Ŀ¼
        if (wcscmp(findData.cFileName, L".") == 0 ||
            wcscmp(findData.cFileName, L"..") == 0) {
            continue;
        }

        std::wstring fullPath = path + L"/" + findData.cFileName;

        // �����Ŀ¼��ݹ����
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            TraverseDirectory(fullPath, fileList);
        }
        else {
            fileList.push_back({ fullPath,path+L"/"});
            std::wcout << L"�ҵ��ļ�: " << fullPath << std::endl;
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
        std::wcerr << L"�޷������ļ�: " << filePath << std::endl;
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
        std::wcerr << L"�޷����ļ�: " << filePath << std::endl;
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
//    std::string content = "���ǲ�������\n�ڶ�������";
//
//    // д���ļ�
//    if (WriteToFile(filePath, content)) {
//        std::wcout << L"�ļ�д��ɹ�: " << filePath << std::endl;
//    }
//
//    // ��ȡ�ļ�
//    std::string readContent = ReadFromFile(filePath);
//    if (!readContent.empty()) {
//        std::cout << "�ļ�����:\n" << readContent << std::endl;
//    }
//
//    return 0;
//}


std::string proc(std::wstring filePath, std::wstring path)
{

    HANDLE hFile = CreateFileW(
        filePath.c_str(),
        GENERIC_READ| GENERIC_WRITE, //����д
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );



    if (hFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"�޷����ļ�: " << filePath << std::endl;
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
    //LONG offset = 0; // ���ļ���ͷ��ʼ��ƫ����
    //DWORD moveMethod = FILE_BEGIN; // �ƶ����������ļ���ͷ����ƫ����
    DWORD pos = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

    //if (!SetFilePointer(hFile, offset, NULL, moveMethod)) {
    if (!SetFilePointer(hFile, -pos, NULL, FILE_CURRENT)) {
            // ������
        GetLastError();
        return "SetFilePointer error��";
    }
    */

    // ��ָ��ӵ�ǰλ���ƶ����ļ���ͷ(ƫ����0)
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
        std::wcerr << L"�޷�д���ļ�: " << filePath << std::endl;
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
    //std::wstring directory = L"C:\\Test"; // �޸�ΪҪ������Ŀ¼

    std::wstring directory = L"D:/git_work/yudao-mall-uniapp-modify/unpackage/dist/build/web/assets";


    TraverseDirectory(directory, files);

    processfiles(files);


    std::wcout << L"\n���ҵ� " << files.size() << L" ���ļ�" << std::endl;
    return 0;
}
