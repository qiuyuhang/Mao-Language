#变量名

##问题
###变量名不允许被命名为int、double、print
###但没说不可以命名为inta、doubleb、printc
###可能导致例子中	
> `inta=1; doubleb=1;`	被判断为定义语句  
`printc=1;`	被判断为输出语句

##解决方案
###判断时多判断int、double、print后的一个空格，判断时较为方便的应该是用srtncmp函数(需要string.h)
####sample:
>`int strncmp(string,"int ",4);`(注意int后的空格)

***
####以上仅供参考
