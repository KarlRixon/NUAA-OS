
<meta charset='utf8'>
<h1>关于复查</h1>
<ol>
  <li>批改成绩时，人工对每题都进行了检查，有的同学做的几乎完全正确，程序中有一点小错误，例如漏掉了初始化变量，这种情况，就不扣分了
  <li>复查时，请先在实验楼中，运行你的程序，如果<b><u>只改动一行代码</u></b>就可以让自己的程序正确运行，可以算对
  <li><a href="https://www.shiyanlou.com/courses/1" target=_blank>实验楼在线虚拟</a>
  <li>申请复查的同学，说明第几题需要复查
  <li>复查时，请今天将请求发给班长，不接受微信私聊。
</ol>

<h1>第一题的评分标准</h1>
<ol>
  <li>编程实现cat /etc/passwd | wc -l &gt;log
  <li>运行程序后，能够在当前目录下创建log文件，并且log文件的内容是/etc/passwd的行数
</ol>

<h1>第二题的评分标准</h1>
<ol>
  <li>使用fork和pipe实现1+2+...100
  <li>创建的10个子进程必须是并行执行的
</ol>

<h1>第三题的评分标准</h1>
<ol>
  <li>使用pthread实现1+2+...100
  <li>不能使用全局变量
</ol>

<h1>第四题的评分标准</h1>
<ol>
  <li>部分同学的输出只能输出put orange/get orange，无法输出put apple/get apple
  <li>我会将程序进行如下调整，经过调整后能够输出put apple/get apple也算正确
<pre>
void *father_entry(void *arg)
{
    将如下代码序列
    ...
    puts("orange");
    sleep(1);
    ...
    pthread_cond_broadcast(&amp;con);

    调整为

    ...
    puts("orange");
    pthread_cond_broadcast(&amp;con);
    ...
    sleep(1);
}
</pre>
</ol>
