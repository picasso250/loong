    IN --------\-/-------- OUT
                |
                |
                |
                |
                | Stack

逆波兰表达式
---

表达式·普通

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

首先，我们有一个输入

    IN: 1 2 3 * +
    ST:

规则也很简单：遇到数字入栈，遇到符号将栈顶的数字运算，并将结果入栈。

IN: 1⇣ 2 3 * +
ST: 

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

SY
===

那么，什么是sy呢？

输入 input(IN)
输出 output(OU)
栈 stack(ST)

IN: 1 + 2 * 3
ST:
OU:

不论如何，我们希望

IN: 1 + 2 * 3
ST:
OU: 1 2 3 * +

我们可能天真的希望
IN: 1 + 2 * 3
OU: 1 2 3 * +

魔法般
我们粗略观察一下，发现可能做不到。
“加入一个间接层，没有什么做不到的。如果有，那就再加一个”

于是我们准备加入一个队列，不，队列太复杂了，我们先试试栈。

第一步

IN: 1⇣ + 2 * 3
ST:
OU:

IN: + 2 * 3
ST:
OU: 1

显然，接下来，2要到输出中去。而加号(+)我们暂时不知道如何处理它。

A：我们需要跳过某个符号的规则。
B：我们先无脑将符号放入栈ST中。
我们先试着选择B方案，看起来简单一些。

IN: 2 * 3
ST: +
OU: 1

> 这里显然有一个规则（规律）：符号一定要先到栈里暂存一下（任何符号这辈子，都要到栈里走一遭的）

> 

无脑

IN: * 3
ST: +
OU: 1 2

再接着肯定还是无脑

IN: 3
ST: + *
OU: 1 2

IN:
ST: + *
OU: 1 2 3

现在，我们得到两个栈，那么我们有两个方案：
A 连接起来
B 依次弹出并压入（从ST中弹出，压入OU）

分别得到两种结果：

1 2 3 + *
1 2 3 * +

分别代表：
1 * (2 + 3)
1 + 2 * 3

看起来，B方案是可取的。

---

说到这里，你可能敏锐的看到一个问题：通过相同的办法，我们会如何处理
1 * 2 + 3

看

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

让我们来回顾一下逆波兰表达式的计算过程（也就是它的本质）

除去什么栈这种术语，我们用大白话回想一下它的处理过程，发现它其实一个消消乐。

1 2 3 * +

从左往右看，发现一个符号(*) 于是，我们可以消去这三个（连数量都和真正的消消乐游戏一样！）

1 [2 3 *] +

消去的结果是6

1 6 +

又遇到可以消去的了

[1 6 +]

7

我们冥冥之中把握到一种整体感，被消去的那些应该是一个整体。

回头看看我们的
失败经历

1 * 2 + 3    ⇨    1 2 3 + *

当进行到这一步的时候

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

这给了我们信心。我们琢磨着，刚才那个乘号(*)有特权（优先权）的原因是什么呢？是因为它先被运算，所以一定在逆波兰表达式中先出现。
这很合理，因为所谓运算符优先级就是这么定义的。

早出现的符号早运算
优先级高的符号应早出现

让我们再瞅一眼命运的分叉路口

IN: 1 * 2 +⇣ 3
ST: * 
OU: 1 2

why + 不可以直接到output？因为+的操作数还没到呢。在逆波兰表达式中，操作总是先于操作符出现的。

rule 1: 操作符总是要到栈中走一遭的。

* 
1 2

rule n: 优先级高的先走。

IN: 1 * 2 +⇣ 3
ST:  
OU: 1 2 *

IN: 1 * 2 + 3⇣
ST: +
OU: 1 2 *

IN: 1 * 2 + 3⇣
ST: 
OU: 1 2 * 3 +

1 * 2 + 3

while (read token from IN) == OK {
    if token is operand {
        push token to OUTPUT;
    } else if token is operator {
        // 让优先级高的先走
        while priority of top of STACK > priority of token {
            Op = pop STACK;
            push Op to OUTPUT;
        }
        push token to STACK;
    }
}
while STACK is not empty {
    Op = pop STACK;
    push Op to OUTPUT;
}

证明：
∵ 1. 所有符号都是先到栈，然后到output
  2. 栈中的符号的优先级总是低于output中的符号
∴ output中的符号是按照优先级从大到小的顺序排列的


IN: a ⊚ b
ST:
OU:

IN: a⇣ ⊚⇣ b⇣
ST: ⊚
OU: a b

IN: a ⊚ b
ST: 
OU: a b ⊚

证明了在output中，操作数总是在操作符的左边

IN: ⟦a⊚b⟧
ST: 
OU: ⟦a⊚b⟧ 

2.

IN: a ⊚ b ⊛⇣ c
ST: ⊚
OU: a b 

如⊚ > ⊛，则 ⊚ 先进去

IN: a ⊚ b ⊛⇣ c
ST: 
OU: a b ⊚

IN: ⟦a⊚b⟧ ⊛ c
ST: 
OU: ⟦a⊚b⟧ 

如⊚ < ⊛，则 压入⊛

IN: a ⊚ b ⊛⇣ c⇣
ST: ⊚ ⊛
OU: a b c

IN: a ⊚ b ⊛ c
ST: ⊚ ⊛⇣
OU: a b c ⊛

IN: a ⊚ ⟦b⊛c⟧
ST: ⊚ 
OU: a ⟦b⊛c⟧



证毕

=========





================================

1 * ( 2 + 3 )

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