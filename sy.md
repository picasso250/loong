

逆波兰表达式
---

表达式•普通

    1+2×3

但是程序员们都习惯将乘号(×)写成 *

我们还可以加个小括号让事情变得更清楚一点

    1+(2*3)

有种表达式叫做前缀表达式、又叫 S表达式

    (+ 1 (* 2 3))

S表达式去掉括号就是 波兰表达式

    + 1 * 2 3

因为发明者是个波兰人

> 逆波兰表达式 就是 将 波兰表达式逆转（错！）

    3 2 * 1 +

开个玩笑

将运算符放在后面边

    (1 (2 3 *) +)

也就是：

    1 2 3 * +

又叫后缀表达式

二维展开！

    ┌─────┬ +
    │ ┌─┬ *   
    1 2 3     

来道大题：

    (1+2)*3
    转换成逆波兰表达式？

答案:

    1 2 + 3 *

这有什么用呢？

特别适合计算机计算：

> 只需要一个栈

首先，我们有一个输入(IN) 有一个栈(ST)

    IN: 1 2 3 * +
    ST:

规则也很简单

> 1. 遇到数字入栈
> 2. 遇到符号将栈顶的数字运算，并将结果入栈。
> 3. 最后剩下来的是结果

举个例子

    IN: 1⇣ 2 3 * +
    ST: 

我们遇到数字 1 于是入栈

    IN: 2 3 * +
    ST: 1

同样的，2也入栈

    IN: 3 * +
    ST: 1 2

3也入栈

    IN: * +
    ST: 1 2 3

然后我们遇到了乘号，那么我们需要将栈顶的数字（们）做一个乘的运算

    2 * 3 = 6

并且我们将6入栈

    IN: +
    ST: 1 6

同样我们计算 1 + 6

    IN:
    ST: 7

当输入（IN）空的时候，栈（ST）中的即是最后结果。

好处是计算机可以无脑从左至右处理。

Shunting Yard
---

那么，什么是Shunting Yard呢？

来个灵魂画风：

                   扳道工在这里哟
                   ↓
    INPUT →→→→→→→→\-/→→→→→→→→ OUTPUT
                   ⇅
                   ⇅
                   ⇅
                   ⇅
                   ⇅ Stack

球球你，把那些横线竖线想象成双轨铁路。
还记得那个灵魂扳道工吗，就是那个决定死一个人还是死五个人的（电车难题）。
对！他所在的场景，就是 Shunting Yard

不过，我们既然决定重新发明Shunting Yard 算法，就先把这个东西放在一边。

我们有

- 输入 input(IN)
- 输出 output(OU)

我们希望输入正确的普通式子，输出正确的逆波兰表达式

也就是

    IN: 1 + 2 * 3
    OU: 1 2 3 * +

我们可能天真的希望

魔法般地，这个天然成立

我们粗略观察一下，发现可能做不到（废话！）。

> “没有什么是加入一个中间层做不到的。如果有，那就再加一个”
>
> ---- 冯•计算机科学大佬•鲁迅•诺伊曼

于是我们准备一个队列，不，队列太复杂了，我们先试试栈。

> 实际上，如果你先试队列，也会得出一样的结论。

第一步

    IN: 1⇣ + 2 * 3
    ST:
    OU:

我们遇到了1，显然，直接放到输出中就可以了。（为什么？因为我们是照着正确的逆波兰表达式照猫画虎的呀）

    IN: + 2 * 3
    ST:
    OU: 1

显然，接下来，输出想要2。而加号(+)我们暂时不知道如何处理它。

> 天使：我们需要跳过某个符号的规则。
>
> 魔鬼：我们先无脑将符号放入栈ST中。

我们先试着选择魔鬼的方案，看起来简单一些。

    IN: 2 * 3
    ST: +
    OU: 1

> 这里显然有一个规则（规律）：符号一定要先到栈里暂存一下（任何符号，这辈子都要到栈里走一遭的）
>
> 魔鬼：任何人到地狱之前，都要到人间走一走的。
>
> 解释：在逆波兰表达式（后缀表达式）中，运算符一定在对应的运算数的后面，而在中缀表达式中，运算符一定在对应数字的中间。一定要让数字先走的话，那么运算符非得在栈中暂时待一会。

于是

    IN: * 3
    ST: +
    OU: 1 2

再接着无脑继续

    IN: 3
    ST: + *
    OU: 1 2

    IN:
    ST: + *
    OU: 1 2 3

现在，我们得到两个栈，那么我们有两个方案：

> 魔鬼：连接起来。
> 
> 天使：依次弹出并压入（从ST中弹出，压入OU）

分别得到两种结果：

    1 2 3 + *
    1 2 3 * +

分别代表：

    1 * (2 + 3)
    1 + 2 * 3

看起来，魔鬼方案是可取的。

---

说到这里，你可能敏锐的看出一个问题：如何处理
1 * 2 + 3 ？

看！

    IN: 1 * 2 + 3
    ST: 
    OU: 

因为你的聪明，我跳过了其中不太关键的步骤

    IN: 2 + 3
    ST: *
    OU: 1

    IN: + 3
    ST: *
    OU: 1 2

    IN:
    ST: * +
    OU: 1 2 3

    IN:
    ST:
    OU: 1 2 3 + *

也就是

    1 * (2 + 3)

问题来了，这和我们说好的 1 * 2 + 3 不一样

让我们来回顾一下逆波兰表达式的本质（也就是它的计算过程）

除去什么“栈”这种术语，我们用大白话回想一下它的处理过程，发现它其实一个 消消乐！

    1 2 3 * +

从左往右看，发现的第一个符号是乘号(*) 于是，我们可以消去这三个（连数量都和真正的消消乐游戏一样！）

    1 [2 3 *] +

消去的结果是6

    1 6 +

又遇到可以消去的了

    [1 6 +]

    7

我们冥冥之中把握到一种整体感：被消去的那些应该是一个整体。

回头看看我们的
失败经历

    1 * 2 + 3    ⇨    1 2 3 + *

当进行到这一步的时候（关键一步！）

    IN: + 3
    ST: *
    OU: 1 2

问题出现了，我们知道 [1 2 *] 是一个整体，也就是 * 现在就要去做 1 和 2 的朋友！立刻！马上！

好吧，我们暂且满足它，将之弹出，并且入OU

    IN: + 3
    ST:
    OU: 1 2 *

接下来继续延续我们之前的套路

    IN: 
    ST: +
    OU: 1 2 * 3

    IN: 
    ST: 
    OU: 1 2 * 3 +

我们（惊讶地）发现，我们竟然做对了

    1 * 2 + 3    ⇨    1 2 * 3 +

这给了我们信心。
我们琢磨着，刚才那个乘号(*)有特权（优先权）的原因是什么呢？是因为它先被运算，所以一定在逆波兰表达式中先出现。
这很合理，因为所谓 **“运算符优先级”** 就是这么定义的。

而在逆波兰表达式中，早出现的符号早运算。

所以
优先级高的符号应早出现

让我们再瞅一眼命运的分叉路口

    IN: 1 * 2 +⇣ 3
    ST: * 
    OU: 1 2

我们似乎找到了另一条规则：

> 规则1 优先级高的先走（对比输入和栈中的符号的优先级，优先级高的直接到输出中去找朋友）
>
> 魔鬼：人分三六九等，等级高的先走也是没毛病的。

但到现在为止，我们都只是把握到一些表面信息，我们需要抽象的思考一下。

回想起来我们的抽象思考大概是从初中的代数开始培养的。那时候，老师说，用字母符号代替数字，是人类数学史上的一次飞跃。

我们也来“飞跃”一下，我们用一些奇怪的符号来代替运算符。

下面的⊚ 和 ⊛代表不同的运算符，可能代表加号，可能代表乘号，可能代表除号。（抽象！）

    IN: a ⊚ b ⊛⇣ c
    ST: ⊚
    OU: a b 

如⊚ > ⊛，就意味着，实际上的式子是这样的：

    (a ⊚ b) ⊛ c

则 ⊚ 先进去找朋友

    IN: a ⊚ b ⊛⇣ c
    ST: ⊚⇣
    OU: a b ⊚

接下来我稍微排个版：

    IN: ⟦a ⊚ b⟧ ⊛ c
    ST: 
    OU: ⟦a b ⊚⟧ 

我们注意双框方括号里的东西，是等价的。

于是，我们用 d 来代替这个双框方括号，于是它变成了最简单的情况：

    IN: d⇣ ⊛ c
    ST: 
    OU: d 

就好像我们直接把d从IN搞到OU一样。

回想一下逆波兰表达式的本质（计算过程），我们确认这是正确的。

如⊚ < ⊛，意味着实际上式子是这样的：

    a ⊚ (b ⊛ c)

接下来看看命运的分岔路口：

    IN: a ⊚ b ⊛⇣ c
    ST: ⊚
    OU: a b 

我们不知所措，因为⊛显然是b和c的朋友，但现在它怎么都不可能偷偷溜到bc的后面，因为bc还未在一起！

那么，简单点 压入⊛，而c肯定是直接去OU了。

    IN: a ⊚ b ⊛⇣ c⇣
    ST: ⊚ ⊛
    OU: a b c

到了这里，我们知道，⊛可以去找朋友了

    IN: a ⊚ b ⊛ c
    ST: ⊚ ⊛⇣
    OU: a b c ⊛

我再偷偷排个版：

    IN: a ⊚ ⟦b ⊛ c⟧
    ST: ⊚ 
    OU: a ⟦b c ⊛⟧

再偷偷用d代换：

    IN: a ⊚ d⇣
    ST: ⊚ 
    OU: a d

哇！

我们再一次正确了！

我们来捋一捋，那条优先级规则的真正执行方式：
1. 将现在要处理的符号和栈顶的符号对比
2. 如果栈顶优先级高，则栈顶的符号弹到输出，并回到 1
3. 否则将要处理的符号入栈

连伪代码我们都可以写出来了：

    while (read Token from INPUT) is OK {
        if Token is operand { # 若是数字
            push Token to OUTPUT
        } else if Token is operator { # 若是符号
            # 让优先级高的先走
            while priority of top of STACK > priority of Token {
                pop STACK to OUTPUT
            }
            push Token to STACK
        }
    }
    while STACK is not empty {
        pop STACK to OUTPUT
    }

其中 pop STACK to OUTPUT 相当于：

    a = STACK.pop()
    OUTPUT.push(a)

括号怎么办？
---

如果含有括号

    1 * ( 2 + 3 )

说到这里我想起一个笑话：

>　　一天，数学家觉得自己已经受够了数学，于是他跑到消防队去宣布他想当消防员。
>
>　　消防队长说：“您看上去不错，可是我得先给您一个测试。”
>
>　　消防队长带数学家到消防队后院小巷，巷子里有一个货栈、一只消火栓和一卷软管。
>
>　　消防队长问：“假设货栈起火，您怎么办？” 　　数学家回答：“我把消火栓接到软管上，打开水龙，把火浇灭。”
>
>　　消防队长说：“完全正确！最后一个问题：假设您走进小巷，而货栈没有起火，您怎么办？”
>
>　　数学家疑惑地思索了半天，终于答道：“我就把货栈点着。”
>
>　　消防队长大叫起来：“什么？太可怕了！您为什么要把货栈点着？”
>
>　　数学家回答：“这样我就把问题简化为一个我已经解决过的问题了。”
>

虽然这个数学家做消防员不称职，可显然，他确实是一名数学家。

我们也要学习一个。

带有括号？

我们先将括号里的转了，这个我们会。

转过之后的结果作为一个整体（看作一个数字）。我们也会。

举个例子：

    IN: 1 * (⇣ 2 + 3 )
    ST: *
    OU: 1

当遇到括号时，我们决定新开一个栈解决当前括号里的式子：

    IN: 2 + 3 
    ST: +
    OU: 2 3

    IN: 2 + 3 
    ST: 
    OU: 2 3 +

于是问题变成了：

    IN: 1 * ⟦2 3 +⟧⇣
    ST: *
    OU: 1

    IN: 1 * ⟦2 3 +⟧⇣
    ST: *
    OU: 1 ⟦2 3 +⟧ 

    IN: 1 * ⟦2 3 +⟧⇣
    ST: 
    OU: 1 ⟦2 3 +⟧ *

结果是 

    1 * ( 2 + 3 )    ⇨    1 2 3 + *

完美！

我已经想出了递归的算法：

    # 这个function和之前的那个天真的算法一模一样
    function naive shunting yard (INPUT) {
        while (read Token from INPUT) is OK {
            if Token is operand { 
                push Token to OUTPUT
            } else if Token is operator {
                while priority of top of STACK > priority of Token {
                    pop STACK to OUTPUT
                }
                push Token to STACK
            }
        }
        while STACK is not empty {
            pop STACK to OUTPUT
        }
    }
    # 下面是带有小括号的算法，用到了上面的代码
    while (read Token from INPUT) is OK {
        # 以下的两个if分支和之前的一毛一样，不用关心
        if Token is operand { 
            push Token to OUTPUT
        } else if Token is operator {
            while priority of top of STACK > priority of Token {
                pop STACK to OUTPUT
            }
            push Token to STACK
        } else if Token is '(' { # 此处！
            # 搜集括号里的东西
            while (read Token from INPUT) is not ')' {
                push Token to NewInput
            } # 此处可以检测一下括号匹配情况，为了防止打扰主线剧情我们就不检测了
            push group (naive shunting yard NewInput) to OUTPUT
        } # 当然你此处你也需要检测一下括号匹配情况，但是我省略了
    }
    while STACK is not empty {
        pop STACK to OUTPUT
    }
    
不过，上面的代码很不优雅（有重复）。

更严重的问题是，想象你是一个古早的计算机科学大佬，你肯定想要找出这个递归算法的对应的迭代版本。

我们来深入的看一下这个计算过程，依然是用简单的例子：

显然我们的算法中，用到了两个栈，我们分别用S₀和 S₁命名。 

当遇到括号时，我们新开了栈(S₁)

    IN: 1 * (⇣ 2 + 3 )
    S₀: * ‖ S₁: 
    OU: 1 ‖

同样的，我们也要新开一个输出。

    IN: 1 * ( 2⇣ + 3 )
    S₀: * ‖ S₁: 
    O₀: 1 ‖ O₁: 2

    IN: 1 * ( 2⇣ + 3 )
    S₀: * ‖ S₁: 
    O₀: 1 ‖ O₁: 2

    IN: 1 * ( 2 +⇣ 3⇣ )
    S₀: * ‖ S₁: +
    O₀: 1 ‖ O₁: 2 3

    IN: 1 * ( 2 +⇣ 3⇣ )
    S₀: * ‖ S₁: 
    O₀: 1 ‖ O₁: 2 3 +

到此为止，我们已经转换完成了括号里的东西。

此时 S₁ 的使命已经完成。对应的标志性事件是：我们遇到了右括号。

    IN: 1 * ( 2 +⇣ 3⇣ )
    S₀: *
    O₀: 1 ‖ O₁: 2 3 +

S₁ 消失了（用完就抛弃人家）。

    IN: 1 * ( 2 +⇣ 3⇣ )
    S₀: *
    O₀: 1 ‖ O₁: 2 3 +

我们惊讶的发现，此时的O₀ 和 O₁ 竟然浑然天成，顺序是对的。

祭出我们的双框括号大法验证一下它确实是正确的。

    IN: 1 * ⟦2+3⟧
    S₀: * 
    O₀: 1 ‖ O₁: ⟦2+3⟧

于是，我们似乎得到了两点启示：

1. 遇到左括号开新栈，遇到右括号让新栈消失
2. 输出可以用同一个输出队列，不会乱。

而且我们可以更进一步：栈也用同一个，不会乱。只要标记新栈开始的地方，然后遇到右括号时，让那个开始标记消失就行了。

理所当然的，我们用左括号'(' 本身来做标记。

于是，我们的伪代码如下：

    while (read Token from INPUT) is OK {
        if Token is "(" {
            push Token to STACK # begin of a virtual stack
        } else if Token is ")" {
            while top of STACK is not "(" {
                pop STACK to OUTPUT
            }
            pop STACK # pop "("
        } else if Token is operand {
            push Token to OUTPUT
        } else if Token is operator {
            # 注意，我们的“新栈”的开头标志是左括号，不要做过界！
            while top of STACK is not "(" and (priority of top of STACK > priority of Token) { 
                pop STACK to OUTPUT
            }
            push Token to STACK
        }
    }
    while STACK is not empty {
        pop STACK to OUTPUT
    }

单目运算符
--------------------------------

其实，以上我们已经重新发明了 shunting yard 算法。不过，我们可以走得更远，我们顺便将之扩展一下，使得它能支持单目运算符。

unary operators

所谓单目运算符，比如

    -1

那个负号就是单目运算符，因为它只有一个操作数。

不过难点在于，负号同时也是减号。

那么，怎么判断它是负号还是减号呢？

略作思考，我们就得出了标准：

>式子的开头，或者紧跟着括号，或者紧跟着其他运算符

                双目
                binary
        ┌───────┤
    - 1 + ( - 2 - - 3 )
    ├───────┴─────┘  
    unary      
    单目         

我们用 -₁ 表示单目运算符
-₂表示双目运算符
那么上面的式子转成逆波兰表达式就是：

    1 -₁ 2 -₁ 3 -₁ -₂ +₂

我们用一个简单的技巧，这个技巧在《C编程语言》这本神书的第一个程序里有用到。

我们将使用State变量来标记式子的开头，或者紧跟着括号与否。

0 表示开头或者紧跟着括号

    # 一开始，式子的开头，肯定要是0
    State = 0
    while (read Token from INPUT) is OK {
        if Token is "(" {
            push Token to STACK
            State = 0 # 紧跟着括号的就是单目运算符
        } else if Token is ")" {
            while top of STACK is not "(" {
                pop STACK to OUTPUT
            }
            pop STACK
            State = 1
        } else if Token is operand {
            push Token to OUTPUT
            State = 1
        } else if Token is operator {
            if State is 0 {
                mark Token as unary
            }
            while top of STACK is not "(" and (priority of top of STACK > priority of Token) { 
                pop STACK to OUTPUT
            }
            push Token to STACK
            State = 0 # 紧跟着其他符号的也是单目运算符
        } 
    }
    while STACK is not empty {
        pop STACK to OUTPUT
    }

下标（c语言中的数组解引用）
-----------------------

也就是这个符号

    []

在c语言中 `a[3]` 表示数组a的3号元素（也就是第4个元素）

我们可以耿直地开始，遇到左方括号还是按照普通符号一般入栈

    IN: a⇣ [⇣ 3⇣ ]
    ST: [
    OU: a 3

等到遇到右方括号，我们可以将一个方括号运算符加入到输出中。

    IN: a [ 3 ]⇣ 
    ST: 
    OU: a 3 []

[] 这个符号，可以看成是一个普通的双目运算符。

伪代码如下：

    State = 0
    while (read Token from INPUT) is OK {
        # 左方括号和普通符号一样处理即可
        # 我们只需要处理右方括号
        if Token is "]" {
            while top of STACK is not "[" {
                pop STACK to OUTPUT
            }
            pop STACK
            push "[]" to OUTPUT
            State = 0
        } else if Token is "(" {
            push Token to STACK
            State = 0
        } else if Token is ")" {
            while top of STACK is not "(" {
                pop STACK to OUTPUT
            }
            pop STACK
            State = 1
        } else if Token is operand {
            push Token to OUTPUT
            State = 1
        } else if Token is operator {
            if State is 0 {
                mark Token as unary
            }
            while top of STACK is not "(" and (priority of top of STACK > priority of Token) { 
                pop STACK to OUTPUT
            }
            push Token to STACK
            State = 0
        } 
    }
    while STACK is not empty {
        pop STACK to OUTPUT
    }

函数调用
--------------------------

接下来，我们遇到了一个有趣的问题：如何支持函数调用？

function call

我们试着耿直地做一下：

    IN: f⇣ (⇣ x⇣ )
    ST: (
    OU: f x

啊，此处触发我们对小括号设置的规则。我们可耻的失败了。

不过，我们虽然撞了南墙，可是可以走得远一些：

    IN: f ( x )⇣
    ST: 
    OU: f x (CALL)

这个 (CALL) 表示函数调用。

这样，在逆波兰表达式中，看到(CALL)，就将栈顶的f和x取出来，调用(CALL)一下。

如果真的是如此就好了。

> 魔鬼：只要看到函数，你就让对应它开启的左括号的右括号消失的时候加个 (CALL) 不就行了？
>
>你：那么怎么知道它是否是函数呢？
>
>魔鬼：函数都是你定义过的，看名字不就知道是否是函数了吗？
>
>天使：有的函数是由高阶函数返回的，不是所有的函数都有名字。
>
>魔鬼：呵呵，就你屁事儿多。那……这样！左括号左边的那位一定是函数。
>
>天使：也可能是运算符
>
>魔鬼：去掉运算符就是函数。
>
>天使：……

真相就是这样，左括号左边的，除了运算符，就是函数。不可能是数字，或者代表数字的变量。

有了标准去判断是否是函数，我们很开心。

    IN: f (⇣ x )

当遇到这种情况时（左括号之前不是运算符），我们就知道f是函数了。

    IN: f (⇣ x⇣ )
    ST: (ᶠᵘⁿ
    OU: f (FUNCTION) x

我在这里将左括号加上fun角标，以便和普通的左括号做区分。

    IN: f ( x )⇣
    ST:
    OU: f (FUNCTION) x (CALL)

当遇到右括号时，如果栈中的左括号是有函数(fun)标记的，则在输出中加入(CALL)

在处理逆波兰表达式的时候，如果遇到(CALL)，则将 (FUNCTION)之后的数字（们）作为参数，将(FUNCTION)之前的那个值作为函数。

但转眼，又遇到了新的问题：多元函数，如f(a,b)该怎么办？

如果耿直一些的话（将逗号的优先级自然地看作很低的优先级），就会变成这么一个局面：

    f ( a , b )
              ⇓
              f (FUNCTION) a b , (CALL)

略微一思索，我们发现，这不是不可以接受，但最好能变成这个局面：

    f (FUNCTION) a b (CALL)

所以，很简单，将输出队列尾部的逗号全都去掉即可。

甚至，我们的规则在面对无参函数时毫无压力

    IN: f (⇣ )
    ST: (
    OU: f (FUNCTION) 

    IN: f ( )⇣
    ST:
    OU: f (FUNCTION) (CALL)

当然，任由参数的表达形式多么复杂，我们的规则都是正确的。

    f(1+2, g(h)(c)) # 这里的g是个高阶函数，返回一个函数
                  ⇓
                  f (FUNCTION) 1 2 + g (FUNCTION) h (CALL) (FUNCTION) c (CALL) (CALL) 

接下来我们考虑伪代码。显然当我们遇到左括号时，需要查看一下前面是不是运算符（不是运算符，就说明前面是函数）

那么现在需要一个新的状态变量。

    State = 0
    IsOp = 1 # 左括号前面是否是符号，没有符号也归类为符号情况进行统一处理
    while (read Token from INPUT) is OK {
        if Token is "]" {
            while top of STACK is not "[" {
                pop STACK to OUTPUT
            }
            pop STACK
            push "[]" to OUTPUT
            State = 0
            IsOp = 0
        } else if Token is "(" {
            if IsOp is 0 { # function call!
                mark Token as fun
                push "(FUNCTION)" to OUTPUT
            }
            push Token to STACK
            State = 0
            IsOp = 0
        } else if Token is ")" {
            while top of STACK is not "(" {
                pop STACK to OUTPUT
            }
            Token = pop STACK
            if Token is fun {
                # 左括号被标记了 fun
                while top of OUTPUT is "," {
                    pop OUTPUT
                }
                push "(CALL)" to OUTPUT
            }
            State = 1
            IsOp = 0
        } else if Token is operand {
            push Token to OUTPUT
            State = 1
            IsOp = 0
        } else if Token is operator {
            if State is 0 {
                mark Token as unary
            }
            while top of STACK is not "(" and (priority of top of STACK > priority of Token) {
                pop STACK to OUTPUT
            }
            push Token to STACK
            State = 0
            IsOp = 1
        }
    }
    while STACK is not empty {
        pop STACK to OUTPUT
    }

结合方向
-----------------------

本来这是应该一开始讲清楚的。可这个问题实在是一个我们已经解决过的问题，所以，我想在最后的时候，顺嘴一提。

还是以c语言为例
点号(.)这个运算符是从左往右计算的。

换言之，点号(.)的结合性是左结合。

如 a . b . c 实际上是 (a . b) . c

而下面两个单目运算符就都是右结合的（且优先级相同）

*&a 实际是 * ( & a )

我们看看那个著名的命运岔路口：

    IN: a . b .⇣ c 
    ST: .
    OU: a b

接下来，不外乎两种情况：

    (a.b).c  √
    a.(b.c)  ×

当然，我们为了做区分，用角标a和b作为区分

    IN: a ⊙ᵃ b ⊙ᵇ⇣ c 
    ST: ⊙ᵃ
    OU: a b

比较 ⊙ᵃ 和 ⊙ᵇ

如果是左结合, ⊙ᵃ 的“优先级更高”

如果是右结合, ⊙ᵇ 的“优先级更高”

当然，他们两个名义上的优先级相同，但毕竟有个先算哪一个的实际问题。

    function real compare operator A and B {
        if priority of A != priority of B {
            return priority of A - priority of B
        } else {
            return A is left associative ? 1 : -1
        }
    }

这是我最后的伪代码啦

    State = 0
    IsOp = 1
    while (read Token from INPUT) is OK {
        if Token is "]" {
            while top of STACK is not "[" {
                pop STACK to OUTPUT
            }
            pop STACK
            push "[]" to OUTPUT
            State = 0
            IsOp = 0
        } else if Token is "(" {
            if IsOp is 0 {
                mark Token as fun
                push "(FUNCTION)" to OUTPUT
            }
            push Token to STACK
            State = 0
            IsOp = 0
        } else if Token is ")" {
            while top of STACK is not "(" {
                pop STACK to OUTPUT
            }
            Token = pop STACK
            if Token is fun {
                while top of OUTPUT is "," {
                    pop OUTPUT
                }
                push "(CALL)" to OUTPUT
            }
            State = 1
            IsOp = 0
        } else if Token is operand {
            push Token to OUTPUT
            State = 1
            IsOp = 0
        } else if Token is operator {
            if State is 0 {
                mark Token as unary
            }
            while top of STACK is not "(" and ((real compare operator (top of STACK) and Token) > 0) {
                pop STACK to OUTPUT
            }
            push Token to STACK
            State = 0
            IsOp = 1
        }
    }
    while STACK is not empty {
        pop STACK to OUTPUT
    }

这一趴纯属作者自己搞怪，你们可以走啦
-----------------------

实际上，我所使用的伪代码是经过仔细设计的（花费了我三分钟呢），是可以使用程序解析的。

我们刨除掉while if 语句什么的，只说两个规则

1. 变量名都是以大写字母开头
2. 函数名中的单词以小写字母开头，可以包括空格，优先匹配函数名字长的。

至于其他的，比如函数调用优先级比运算符优先级高，和c语言是一样的。

函数的定义方式如下：

    function verb1 verb2 (Var1) conj1 (Var2) maybe suffix {
        ...
    }

说了那么多废话，我只是想引出一个问题：如何用Shunting Yard 算法解析这门语言的表达式？

答案：

重点其实是改变逆波兰表达式的求值方式。。。

1. using space as operator, priority of space is as high as "( )"
2. scan, if SUCCESS, mark it
3. scan from right, call SUCCESS
4. sub it with its result
5. go to 2

例子：

    IN: top of STACK is not "("
    OU: top of (SPACE) STACK (SPACE) is (SPACE) not (SPACE) "(" (SPACE)

    ST: top of (SPACE) STACK (SPACE) (SUCCESS) is (SPACE) not (SPACE) "(" (SPACE)

    ST: A is (SPACE) not (SPACE) "(" (SPACE)

    ST: B
