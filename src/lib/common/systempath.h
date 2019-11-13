

#include <QString>

class SystemPath
{

public:

    //获取工程文件路径
    static QString getProjectRootPath();
    static QString getCurrentPath();
    static QString getResourcePath();
    static QString getFilesPath();
    static QString getPluginPath();

};

