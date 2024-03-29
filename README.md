## 项目名称
C++ 版飞鸽传书，在 `Ubuntu 22.04.3 LTS` 开发，基于 TCP 传输文件，通过 UDP 收发消息

## 项目特点
1. 查看在线用户
2. 收发信息
3. 收发文件
4. 自动保存聊天记录
5. 查阅聊天记录

## 运行条件
- Linux 环境，应安装软件包 `net-tools`
- C/C++ 标准：`C++17` 及以上
- 对当前目录有写权限
- 对要发送的文件有读权限
- 该程序会产生其他文件/文件夹，包括日志文件、接收到文件的文件夹，请单独为该程序建立文件夹

## 运行说明
编译命令示例：请根据实际调整
```bash
g++ -g /PATH/TO/SRC/*.cpp -o /PATH/TO/ipmsg/bin/ipmsg
```
