## 项目名称
C++ 版飞鸽传书，基于 TCP 传输文件，通过 UDP 发送消息

## 运行条件
- Linux 环境，在 `Ubuntu 22.04.3` 开发
- C++ 标准：`c++17` 及以上
- 对当前目录有写权限
- 对要发送的文件有读权限

## 运行说明
编译命令示例：可根据需要调整
```bash
g++ -g ${fileDirname}/*.cpp -o ${fileDirname}/../bin/${fileBasenameNoExtension}
```
