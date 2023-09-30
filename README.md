# 网站留言评论系统-初稿(simple-comment-system-pre)
## 概述
此项目为从`计算机入门水平`升阶为`计算机进阶-前期水平`的升阶项目的**初稿**部分，以简单、简洁性和完整性为主，除了必要的数据库API外，没有额外的外部插件库，全是由个人手动实现的底层源码。

此处的C语言指C，不包含C++，使用的C标准在C99之下(不包括C99)。<br/>
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

在该项目的介绍文章中你能感受到下列所述内容：设计思路、分析思路、如何寻找思路、如何改善思路、通过思路如何设计、设计过程中出现问题如何解决问题、实现过程中如何确定应用那些知识等。

详细介绍：[网站留言评论系统-初稿的(思路与教程)文章](https://www.reversesacle.com/computer-science/programming/c-language/project/comment-system-part1/)
## 涉及的计算机知识范围
+ [C语言基础知识](https://www.reversesacle.com/computer-science/programming/c-language/basis/c/cpart1/)
+ [计算机网络(运输层和应用层)基础知识](https://www.reversesacle.com/computer-science/computer-basic/network/nwpart3/)
+ [HTML与CSS基础知识](https://www.reversesacle.com/computer-science/programming/web/hcpart1/)
+ [JavaScript基础知识](https://www.reversesacle.com/computer-science/programming/web/jspart1/)
+ [有关网络编程的全基础知识(额外包含了Linux环境编程/GCC编译器/GDB调试器)](https://www.reversesacle.com/tags/network-programming/)
+ [HTTP网络通信基础](https://www.reversesacle.com/computer-science/programming/c-language/network-programming/http/)
+ [数据库基础知识与MySQL数据库基础](https://www.reversesacle.com/tags/database/)
## 源码使用流程详解
首先，阅览[→Linux(CentOS 7)](https://www.reversesacle.com/computer-science/programming/c-language/network-programming/Linux-basic/)，从**准备条件**部分到**通过客户端连接服务器**部分，以及额外的**客户端(SFTP)命令**部分。

接着，确保Linux虚拟机环境安装了make工具，倘若没有可使用命令 `yum -y install make`

之后，获取源码(Clone地址：`https://github.com/ReverseSacle/simple-comment-system-pre.git`)，通过`SecureCRT`软件连接Linux虚拟机环境，连接之后在顶栏标签处右键选`Connect SFTP Session`来通过SFTP命令向Linux虚拟机环境传入文件。

**目录结构**

httpserver
- docs
  - css
    - comments.css
  - img
    - img.png
  - js
    - comments.js
  - index.html
- _clibs.h
- _clibs.c
- database.h
- database.c
- http.h
- http.c
- httpserver.c

其中`httpserver`为根目录，docs目录以及其以内的目录可通过`cd`与`mkdir`Linux命令的配合来创建，倘若通过SFTP传入的文件位置不对，可通过`mv 文件名 目的路径`命令来移动位置。

(**注意**)需预先安装MySQL，详细看[该项目介绍文章第一部分](https://www.reversesacle.com/computer-science/programming/c-language/project/comment-system-part1/)的**Linux服务器处理进程的数据库链接**内容。

此外，还需修改`comments.js`文件`window.onload`开头的`_host`变量的值，改为`SecureCRT`连接的IP地址。

最后，在`httpserver`根目录中执行命令`make`，接着执行`./httpserver`即可启动服务端进程，之后在浏览器中的输入栏输入`http://(SecureCRT连接的IP地址)/index.html`，回车即可看到评论系统的前端界面。
## 源码解析和教程更新中...
