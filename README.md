# WinGraph
A GDI+ WinGraph Implementation

来自 VCG : Visualization of Compiler Graphs 
原URL我就不贴了，自己搜。
这里只是简单用MFC 和 GDI+实现绘制流程图等，程序完全抄自IDA Pro的WinGraph，只是不用QT了而已。
可能会有问题，懒得修改。


程序将GDL语言文件编译后成图形后画出来，这就是IDA 中 Function Call （函数调用关系图）

为减少体积，方便阅读，只保留了window是平台相关的代码和注释。
