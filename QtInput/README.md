# qt虚拟输入法，可输入中文，可用于ARM-LINUX

1. 先编译googlepinyin（注意选择release），生成libgooglepinyin.a，将改文件放到plugin/googlepinyin文件夹下
2. 编译plugin，生成libtgtsmlInputContextPlugin.so，（注意不要复制链接文件,终端输入ls -la查看链接文件），将该文件复制到开发板qt的plugins/platforminputcontexts文件夹下，若无platforminputcontexts文件夹，则手动建立
3. 将plugin下的dict文件夹放到工程编译生成的APP同一目录下
4. 在工程的main.cpp的主函数中添加qputenv("QT_IM_MODULE", QByteArray("tgtsml"));，注意放到QApplication a(argc, argv);之前
5. 运行APP，点击文本框，即可弹出输入法

教程链接:

 https://blog.csdn.net/qq_32605451/article/details/107705710

原链接：

 https://gitee.com/smartwell/QtInputMethod_GooglePinyin?_from=gitee_search 
