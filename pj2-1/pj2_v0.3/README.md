## 使用
请确认`FILE_root`和`USER_root`文件夹，以及`user_permission_file.csv`在当前目录下，运行`main.cpp`即可在命令行中输入user id和file id来查询permission。

## 文件结构
- `LoadData.hpp`中存放读取文件树、用户树以及permission file的函数
- `Permission.hpp`中定义了`Permission 类`
- `Tree.hpp`中定义了`UserTree`类和`FileTree`类
- `main.cpp`中实现了命令行交互来查询permission

## 项目的特性
- 采用了双层Hash表来储存和查找permission，提高程序效率
- 采用了数组来储存用户树和文件树，因为没有对权限进行修改的需求，节省内存
- 采用了Hash 表来从名字查找`ID`，提高了程序效率
