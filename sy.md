

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
    转换成波兰表达式？

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

> 这里显然有一个规则（规律）：符号一定要先到栈里暂存一下（任何符号这辈子，都要到栈里走一遭的）

> 魔鬼：任何人到地狱之前，都要到人间走一走的。

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

> 天使：连接起来
> 
> 魔鬼：依次弹出并压入（从ST中弹出，压入OU）

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

早出现的符号早运算
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

    IN: 1 * (⇣ 2 + 3 )
    ST: *
    OU: 1
    
IN: 1 * (⇣ 2 + 3 )
S₀: * ‖ S₁: 
OU: 1 ‖

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

IN: 1 * ⟦2+3⟧
S₀: * ‖ S₁: 
O₀: 1 ‖ O₁: ⟦2+3⟧

IN: 1 * ⟦2+3⟧ ...
S₀: * ‖ S₁: ‖ S₂
O₀: 1 ‖ O₁: ⟦2+3⟧ ‖ O₂

IN: 1 * ⟦2+3⟧ ...
S₀: *
O₀: 1 ⟦2+3⟧ 

let's go back to

IN: 1 * (⇣ 2 + 3 )
S₀: * 
OU: 1 

IN: 1 * (⇣ 2 + 3 )
S₀: * (
OU: 1 

IN: 1 * ( 2 + 3 )⇣
S₀: * ( +
OU: 1 2 3 

IN: 1 * ( 2 + 3 )⇣
S₀: * ( 
OU: 1 2 3 +

IN: 1 * ( 2 + 3 )
S₀: *  
OU: 1 2 3 +

IN: 1 * ( 2 + 3 )
S₀: 
OU: 1 2 3 + *

1 2 3 + *  -->  1 * ( 2 + 3 )

priority of "(" = -∞;
while (read token from IN) == OK {
    if token is operand {
        push token to OUTPUT;
    } else if token is operator {
        // '(' is smallest, '(' is barrier
        while priority of top of STACK > priority of token { 
            Op = pop STACK;
            push Op to OUTPUT;
        }
        push token to STACK;
    } else if token is "(" {
        push token to STACK; // begin of a virtual stack
    } else if token is ")" {
        while top of STACK is not "(" {
            Op = pop STACK;
            push Op to OUTPUT;
        }
        pop STACK; // pop "("
    }
}
while STACK is not empty {
    Op = pop STACK;
    push Op to OUTPUT;
}


--------------------------------

unary operators

start or start of () or []

- 1 + ( - 2 - 3 )
│───────┘   │
unary       binary


priority of "(" = -∞;
// 0 is initial state
state = 0; // 0 is start or start of () or [], 1 else
while (read token from IN) == OK {
    if token is operand {
        push token to OUTPUT;
        state = 1;
    } else if token is operator {
        if state == 0 {
            mark operator as unary;
        }
        while priority of top of STACK > priority of token { 
            Op = pop STACK;
            push Op to OUTPUT;
        }
        push token to STACK;
        state = 1;
    } else if token is "(" {
        push token to STACK;
        state = 0;
    } else if token is ")" {
        while top of STACK is not "(" {
            Op = pop STACK;
            push Op to OUTPUT;
        }
        pop STACK;
        state = 1;
    }
}
while STACK is not empty {
    Op = pop STACK;
    push Op to OUTPUT;
}

-----------------------

[]

IN: a⇣ [⇣ 3⇣ ]
ST: [
OU: a 3

IN: a [ 3 ]⇣ 
ST: 
OU: a 3 []

--------------------------

function call

IN: f⇣ (⇣ x⇣ )
ST: (
OU: f x

IN: f ( x )⇣
ST: 
OU: f x (CALL)

f ( a , b )
⇓
f a b , (CALL)

IN: f⇣ (⇣ a⇣ ,⇣ b⇣ ,⇣ c⇣ )
ST: ( , ,
OU: f a b c

IN: f⇣ (⇣ a⇣ ,⇣ b⇣ ,⇣ c+d⇣ )
ST: ( , , +
OU: f a b c d

IN: f ( a , b , c )⇣
ST: 
OU: f a b c ,³ 

IN: f ( a , b , c )⇣
ST: // I'm empty
OU: f a b c ,³ (CALL)

IN: f ( a , b , c )⇣
ST: (CALL)
OU: f a b c ,³ (CALL)

IN: f (⇣ a , b , c )
ST: (CALL) (
OU: f 

... else if token == "(" {
    if state == 1 {
        push (CALL) to STACK
    }
    push "(" to STACK
} ...

// meet ")"
n = 1;
while (token = pop stack) != "(" {
    if token is "," {
        n++;
    }
}
push ",ⁿ" to STACK

⚠️
g + f ( )
⇓
g f ,¹ (CALL) +      
⚠️

// meet ")"
n = 0;
if state != 0 { // not start of (
    n = 1;
}
while (token = pop stack) != "(" {
    if token is "," {
        n++;
    }
}
if top of STACK == "(CALL)" {
    push (CALL)ⁿ to STACK;
} else {
    push ,ⁿ to STACK;
}

g + f ( )
⇓
g f (CALL)⁰ +   

(*fp)(1,3)

-----------------------

结合方向

a . b . c 

foo.bar()

IN: a . b .⇣ c 
ST: .
OU: a b

(a.b).c  √
a.(b.c)  ×

结合性，谁先和谁结合

从左边开始算，就叫左结合。

*&a

IN: a ⊙ b ⊙ᵇ⇣ c 
ST: ⊙ᵃ
OU: a b

compare ⊙ᵃ and ⊙ᵇ
left, ⊙ᵃ is higher
right, ⊙ᵇ is higher

func compare(a, b) {
    if priority of a != priority of b {
        return a - b;
    } else {
        return a is left associative ? 1 : -1;
    }
}

-----------------------
3+a[4]
+[]
3a


priority of "(" = -∞;
priority of "[" = -∞;
priority of "[]" = ∞;
priority of "(CALL)" = ∞;
// 0 is initial state
state = 0; // 0 is start or start of () or [], 1 else
while (read token from IN) == OK {
    if token is operand {
        push token to OUTPUT;
        state = 1;
    } else if token is operator {
        if state == 0 {
            mark operator as unary;
        }
        while priority of top of STACK > priority of token {
            Op = pop STACK;
            push Op to OUTPUT;
        }
        push token to STACK;
        state = 1;
    } else if token is "(" {
        if state == 1 {
            push (CALL) to STACK;
        }
        push token to STACK;
        state = 0;
    } else if token is ")" {
        n = 0;
        if state == 1 {
            n = 1;
        }
        while top of STACK is not "(" {
            if top of STACK is "," {
                n++;
                pop STACK;
            } else {
                Op = pop STACK;
                push Op to OUTPUT;
            }
        }
        pop STACK;
        if top of STACK == "(CALL)" {
            pop STACK;
            push (CALL)ⁿ to OUTPUT;
        } else {
            push ,ⁿ to OUTPUT;
        }
        state = 1;
    } else if token is "[" {
        push "[]" to STACK;
        push token to STACK;
        state = 0;
    } else if token is "]" {
        while top of STACK is not "[" {
            Op = pop STACK;
            push Op to OUTPUT;
        }
        pop STACK;
        state = 1;
    }
}
while STACK is not empty {
    Op = pop STACK;
    push Op to OUTPUT;
}