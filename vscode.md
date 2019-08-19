### 使用 vscode 搭建 linux 开发环境

---

#### 1. 工具下载、插件安装 

 - 下载 vscode

 - 安装插件 Remote-SSH

   ![1563776832011](C:\Users\shirao.wang\AppData\Roaming\Typora\typora-user-images\1563776832011.png)

#### 2. 配置 vscode 远程连接 linux 机器

- vscode 使用 ssh 连接到 linux 机器需要使用 sshkey，我们可以直接使用之前搭建 git 环境时的 key 

- 详细步骤

  - 打开 vscode，按 `F1` 打开输入框，输入 `Remote-SSH:Open Configuration File` 打开用户目录下的 `.ssh\config` 文件

    ![1563777234053](C:\Users\shirao.wang\AppData\Roaming\Typora\typora-user-images\1563777234053.png)

    ![1563777252645](C:\Users\shirao.wang\AppData\Roaming\Typora\typora-user-images\1563777252645.png)

  - 把 linux 机器的 ip 和用户名添加到上一步的 config 配置中，可以添加多个，如图：

    ![1563777431830](C:\Users\shirao.wang\AppData\Roaming\Typora\typora-user-images\1563777431830.png)

  - 把 public SSH key 添加到 linux 机器的 `~/.ssh/authorized_keys` 中。进入用户下的 .ssh 目录，右键打开 git 终端，输入 `ssh-copy-id [-i [identity_file]] [user@]machine`，如图：

    ![1563778030524](C:\Users\shirao.wang\AppData\Roaming\Typora\typora-user-images\1563778030524.png)

    这里的 user 和 machine 要和上一步配置的对应，回车后输入 linux 机器上该用户的密码

  - 此时，vscode 已经远程连接到远程机器上，可以用 vscode 进行编码了

    ![1563778579948](C:\Users\shirao.wang\AppData\Roaming\Typora\typora-user-images\1563778579948.png)

#### 3. C++ 插件安装、调试

- 安装插件：C/C++，用途：补全、提示、高亮、debug

- 配置调试环境，支持 GDB 调试，支持 `F9` 断点调试

  - 按 `F1` 打开输入框，输入 `launch.json`，回车打开配置文件，点击 **添加配置**，选择 C/C++:(gdb) Attach   把 “program” 选项设置为对应的可执行文件目录，“name” 选项随意设置，建议设置为 server 名
  - ![1563779199879](C:\Users\shirao.wang\AppData\Roaming\Typora\typora-user-images\1563779199879.png)

- 开始调试

  - 点击左边的 “爬虫” 图标，选择一个配置

    ![1563779335107](C:\Users\shirao.wang\AppData\Roaming\Typora\typora-user-images\1563779335107.png)

  - 点击按钮或者按 F5，在弹出框中输入要调试的进程，如：gameserver

  - F9 下断点、F10、F11单步执行，一切都和 vs 一样，开始调试后会打开一个调试窗口，也可以使用 gdb 命令调试

    ![1563779609994](C:\Users\shirao.wang\AppData\Roaming\Typora\typora-user-images\1563779609994.png)

#### 4. 其他

- 其他推荐安装的插件

  ![1563780108672](C:\Users\shirao.wang\AppData\Roaming\Typora\typora-user-images\1563780108672.png)

