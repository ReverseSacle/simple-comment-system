# 网站留言评论系统(simple-comment-system-pre)

## 预览

![](./preview.png)

## 概述

此项目为从`计算机入门水平`升阶为`计算机进阶-前期水平`的升阶项目。

**初稿**部分以简单、简洁性和完整性为主，除了必要的数据库API外，没有额外的外部插件库，全是由个人手动实现的底层C语言源码。

对于初稿所指的C语言指C，不包含C++，使用的C标准在C99之下(不包括C99)。<br/>
该项目在**Linux系统CentOS 7**下编译，测试和运行，具体操作包含

+ **HTML和CSS的源码**编写
+ **JavaScript脚本语言监听元素事件的源码**编写
+ **支持http1.0协议的socket通信的纯C语言源码**编写
+ **MySQL数据库(C语言API)的数据库处理纯C语言源码**编写
+ **前端网络请求的JS源码**编写
+ **后端对前端请求的处理纯C语言源码**编写
+ **前端通过后端与数据库进行数据交换的纯C语言源码和JS源码**编写
+ **前端与后端与数据库的动态化**处理
+ 服务器进程源码的**多线程模式**编写
+ 服务器进程源码的**日志监听模式**编写
+ 对后端服务器进程纯C语言源码进行**高性能化**的处理

**中稿**部分以附加功能为主，主要是将编程语言切换到cpp，并利用了cpp中的第三方库添加并改善了相关功能。

**设计思路与源码解析，以及项目实现的全过程** - [网站留言评论系统-初稿与中稿的(思路与教程)文章](https://www.reversesacle.com/computer-science/programming/c-language/c-project/comment-system-part1/)

## 分支说明

- `main` - 主线分支，由`C++`编写，通过使用外部库及API来对功能进行优化，并作为主线来更新其他功能
- `original` - 最初的初稿成品，由C语言编写
- `original-cpp` - 最初的初稿成品，由`C++`编写

## 初稿涉及的计算机知识范围

+ [C语言基础知识](https://www.reversesacle.com/computer-science/programming/c-language/basis/c/cpart1/)
+ [计算机网络(运输层和应用层)基础知识](https://www.reversesacle.com/computer-science/computer-basic/network/nwpart3/)
+ [HTML与CSS基础知识](https://www.reversesacle.com/computer-science/programming/web/hcpart1/)
+ [JavaScript基础知识](https://www.reversesacle.com/computer-science/programming/web/jspart1/)
+ [有关网络编程的全基础知识(额外包含了Linux环境编程/GCC编译器/GDB调试器)](https://www.reversesacle.com/tags/network-programming/)
+ [HTTP网络通信基础](https://www.reversesacle.com/computer-science/programming/c-language/network-programming/http/)
+ [数据库基础知识与MySQL数据库基础](https://www.reversesacle.com/tags/database/)

## 源码使用流程详解

**目录结构**

```tex
httpserver
├── _config
│   ├── _config.cc
│   ├── _config.h
│   ├── _config.json
│   └── json.hpp
├── _database
│   ├── db.cc
│   └── db.h
├── docs
│   ├── css
│   │   ├── comment_reply.css
│   │   └── comments.css
│   ├── index.html
│   └── js
│       └── comments.js
├── _http
│   ├── http.cc
│   ├── http.h
│   ├── _response.cc
│   └── _response.h
├── httpserver.cc
├── makefile
├── mylibs.cc
├── mylibs.h
└── _tcp
    ├── tcp.cc
    └── tcp.h
```

**获取源码**

```bash
git clone https://github.com/ReverseSacle/simple-comment-system.git
```

**正式操作**

(**info**) 这里以二进制的方式运行，默认在虚拟机上Linux系统(CentOS7)中运行。

(**info**) 编译前需预先安装MySQL，详细请看[→该项目介绍文章第一部分](https://www.reversesacle.com/computer-science/programming/c-language/project/comment-system-part1/)的【**Linux服务器处理进程的数据库链接**】区块。

(**info**) 运行前需修改`./docs/js/comments.js`文件开头的`_host`变量的值，改为虚拟机的IP地址。

```bash
# (可选)预先确保Linux虚拟机环境安装了make工具
yum -y install make

# 配置库连接位置
vi /etc/ld.so.conf
## 新的一行添加 /usr/local/lib
## 输入 :x
## 退出后输入命令 ldconfig

cd simple-comment-system

make httptest
## make操作详细操作可查看makefile文件
```

可通过`ctrl c`中断，之后直接执行`./httpserver`即可启动服务端进程，再在浏览器中的输入栏输入`http://(虚拟机IP地址)/index.html`，回车即可看到评论系统的前端界面。

## 第三方依赖

- [mysql++](https://tangentsoft.com/mysqlpp/wiki?name=MySQL%2B%2B&p&nsm) - 用于mysql数据库的相关操作(`C++`)
- [gabime/spdlog](https://github.com/gabime/spdlog) - 用于记录日志(`C++`)
- [nlohmann/json](https://github.com/nlohmann/json) - 用于json文件的解析(`C++`)
- `crypto-js` - 用于SHA256的哈希处理(JavaScript)

  ```html
  <script type="text/javascript" src="https://cdn.jsdelivr.net/npm/crypto-js@4.2.0/core.js"></script>
  <script type="text/javascript" src="https://cdn.jsdelivr.net/npm/crypto-js@4.2.0/sha256.js"></script>
  ```

## Record

- 使用了`gabime/spdlog`第三方库来配置日志功能 - `tag v0.0.2`
- 更换不可更改配置的运行方法，采用运行时动态解析JSON文件的方式来获取配置，使用了`nlohmann/json`第三库来解析JSON配置文件 - `tag v0.0.3`
- 完善之前的改进部分并优化项目代码结构，使用`C++11`特性改善了代码 - `tag v0.0.4`
- 更改数据库的存储数据类型，并利用`crypto-js`进行SHA256的哈希处理，以此适配avatar头像。部分代码的结构进行了调整 - `tag v0.0.5`
- 更改数据库的表结构，添加了评论回复功能的前端样式、前端回复功能的交互代码、后端交互代码和服务器后端与前端的交互代码，之后，对前端的样式代码进行了文件拆分，对前端的js代码进行了重构 - `tag v0.0.6`
- 添加了回复对象的显示，并对所有代码进行了简明的注释 - `tag v0.0.7`
