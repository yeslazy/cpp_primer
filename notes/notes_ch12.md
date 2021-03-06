# 第12章 动态内存

- 静态内存：保存局部static对象、类static数据成员、定义在任何函数之外的变量
- 栈内存：保存定义在函数内的非static对象；
- 保存在静态内存和栈内存中的对象由编译器自动创建和销毁
- 栈内存中的对象在其定义的程序块运行时才存在
- static对象在使用之前分配，程序结束时销毁
- **自由空间**或**堆**：用来存储动态分配的对象的内存空间



## 12.1 动态内存与智能指针

- 动态内存管理
  - 运算符**new**：在动态内存中为对象分配空间并返回一个指向该对象的指针
  - 运算符**delete**：接受一个动态对象的指针，销毁该对象，释放相关内存
- 标准库在**memory**头文件中定义的智能指针
  - 智能指针是模板，必须像`vector`一样提供额外信息；
  - 指针指针负责自动释放指向的对象
  - **shared_ptr**：允许多个指针指向同一个对象
  - **unique_ptr**：独占所指向的对象
  - **weak_ptr伴随类**：它是一个弱引用，指向`shared_ptr`所管理的对象

### 12.1.1 shared_ptr类

- `shared_ptr`和`unique_ptr`都支持的操作
  - 空智能指针，可以指向类型为`T`的对象：`shared_ptr<T> sp;`或者`unique_ptr<T> up`
  - 智能指针`p`用作一个条件潘顿，若`p`指向一个对象，则为`true`
  - 解引用智能指针`p`，即`*p`时，获得`p`指向的对象
  - `p->mem`等价于`(*p).mem`
  - 获得指针指针`p`中保存 指针：`p.get()`
  - 交换智能指针`p`和`q`中的指针
    - 算法函数方法：`swap(p, q)`
    - 成员函数方法：`p.swap(q)`
- `make_shared`函数分配对象并初始化
  - 在**memory**头文件中定义
  - `make_shared<T>(args)`
    - 返回一个`shared_ptr`，指向一个动态分配的类型为T的对象
    - 使用`args`作为T类型构造函数的输入，初始化对象
    - 当不传递任何`args`参数，对象会进行**值初始化**；即内置类型自动设置为0，类类型则由类默认初始化
- `shared_ptr`的拷贝与赋值
  - 每个`shared_ptr`有一个关联的引用计数。
  - 拷贝一个`shared_ptr`时，引用计数增加，具体包括
    - 用一个`shared_ptr`初始化另一个`shared_ptr`
    - 作为参数传递给一个函数
    - 作为函数的返回值
  - 引用计数递减情况
    - 给`shared_ptr`赋予一个新值
    - `shared_ptr`被销毁
  - 当引用计数变为0，则它会自动释放自己管理的对象
- `shared_ptr`自动销毁所管理的对象
  - 使用析构函数完成销毁的工作
  - 析构函数一般用来释放对象所分配的资源
- `shared_ptr`还会自动释放相关的内存
  - 如果智能指针`shared_ptr`被释放，且没有其他`shared_ptr`也指向这块内存，则会自动释放`shared_ptr`管理的动态内存
  - 如果将`shared_ptr`放在一个**容器**中，而后不再需要全部元素，而用其中一部分，要**记得`erase`删除不再需要的哪些元素**
- 使用动态内存的三个原因
  - 程序不知道自己需要使用多少对象——容器类
  - 程序不知道所需对象的准确类型——模板
  - 程序需要在多个对象间共享数据——智能指针

### 12.1.2 直接管理内存

- 运算符`new`分配内存；运算符`delete`释放`new`分配的内存
- 自己直接刾内存的类蹦年依赖类对象的拷贝、赋值和销毁操作的任何默认定义

- 使用`new`动态分配和初始化对象
  - 默认情况下，动态分配的对象是默认初始化，即内置类型或组合类型的对象的值是未定义的，类类型对象使用默认构造函数进行初始化
  - 直接初始化方式
    - 圆括号传统的构造方式：`int *pi = new int(1024)`
    - 花括号列表初始化：`vector<int> *pv = new vector<int>{0, 1, 2}`
    - 空括号进行值初始化：`int *p =  new int()`
  - 对于类类型，值初始化使用默认构造函数来初始化。
  - 当**单一初始化器**时，可以使用`auto`从初始化器推断：`auto p1 = new auto(obj)`；p指向obj相同的类型
- 动态分配的const对象：`const int *pi = new const int(1024)`
- 内存耗尽
  - 分配内存空间不成功，抛出bad_alloc异常
  - 阻止抛出异常：`int *pi = new (nothrow) int`
    - 这种形式称为**定位new**
    - **nothrow**是一个对象
- delete表达式
  - 销毁给定指针指向的**动态分配**的对象，释放对应的动态内存
  - 未定义的行为
    - 释放并非new分配的内存
    - 将同一个的指针值释放多次
- 动态对象的生存期直到被显示释放之前它都是存在的
- **空悬指针**
  - 定义：指向一块曾经保存数据对象但现在已经无效的内存的指针
  - 产生：释放内存后，指针指向的内存被释放，但指针仍然存在
  - 避免问题的方法
    - 方法1：在指针即将离开其作用域之前释放掉它所关联的内存
    - 方法2：释放内存后，指针赋值为`nullptr`
- 多个内置指针指向相同的动态内存对象，设置一个为`nullptr`后，其他内置指针数值仍然不会变

### 12.1.3 shared_ptr和new结合使用

- 可以用new返回的指针初始化智能指针
- 接受指针参数的智能指针构造函数是**explicit**
  - 不能将一个内置指针隐式转换为一个智能指针：`shared_ptr<int> p1 = new int(1024)`是错误的
  - 必须使用直接初始化形式：`shared_ptr<int> p2(new int(1024))`
- 不要混合使用普通指针和智能指针
  - 推荐使用`make_shared`，不使用`new`
  - 避免无意中将同一块内存绑定到多个独立创建的`shared_ptr`上
  - 当将一个`shared_ptr`绑定到一个普通指针时，我们即将内存的管理责任交给了这个`shared_ptr`。一旦这样做了，我们就不应该再使用内置指针来访问`shared_ptr`所指向的内存了。
- 不要使用`get`初始化另一个智能指针或为智能指针赋值
  - `get`返回的指针的代码不能`delete`此指针
- 其他`shared_ptr`操作
  - 初始化`shared_ptr`对象
    - `shared_ptr<T> p(q)`：`p`管理内置指针`q`所指向的对象；`q`必须指向new分配的内存且能够转换为`T*`类型
    - `shared_ptr<T> p(u)`：`p`从`unique_ptr u`那里接管了对象的所有权；将`u`置位空
    - `shared_ptr<T> p(q, d)`：`p`接管了内置指针`q`所指向的对象的所有权。`q`必须能转换为`T*`类型。`p`将使用可调对象`d`来代替delete
    - `shared_ptr<T> p(p2, d)`：`p`是`shared_ptr p2`的拷贝，使用可调对像`d`代替delete
  - `reset`函数将内置指针赋值给`shared_ptr`
    - `p.reset()`
    - `p.reset(q)`
    - `p.reset (q, d)`
    - 若`p`是唯一指向其对象的`shared_ptr`，则释放此对象
    - 若传递了内置指针`q`，则令`p`指向`q`，否则会将`p`置为空
    - 若还传递了参数`d`，则会调用`d`而不是`delete`来释放`q`

### 12.1.4 智能指针和异常

- 智能指针能在发生异常且不能很好捕获异常时候，在代码块结束时释放动态内存
- 有些类需要显式释放所使用的任何资源；通过智能指针实现
  - connection在结束时候，需要disconnect函数释放资源
  - 定义删除器函数：`void end_connection(connection * p) {disconnect(*p)}`
  - 定义指针指针时传递删除器函数：`shared_ptr<connection> p (&c, end_connection)`
  - 当指针指针`p`被销毁时，调用end_connection函数代替delete操作符，即调用disconnect函数释放connection资源
- 智能指针注意事项
  - 不使用相同内置指针初始化多个智能指针
  - 不delete使用get函数返回的指针
  - 不使用get函数初始化或reset另一个智能指针
  - 智能指针管理不是new分配的内存，记住传递删除器

### 12.1.5 unique_ptr

- `unique_ptr`独自拥有指向的对象
- 定义`unique_ptr`时需要将其绑定到一个new返回的指针
- 初始化unique_ptr必须采用**直接初始化**
- 不支持普通的拷贝或赋值操作
  - 不支持拷贝：`unique_ptr<string> p2(p1)`
  - 不支持赋值：`p3 = p1`
- 初始化
  - `unique_ptr<T> u1`：空unique_ptr，可指向T类型的对象，u1使用delete释放指针
  - `unique_ptr<T, D> u2`：空unique_ptr，可指向T类型的对象，使用类型为D的可调用对象释放指针
  - `unique_ptr<T, D> u(d)`：空unique_ptr，指向T类型对象，使用类型为D的对象d代替delete
- 通过release或reset将指针的是所有权从一个非const的unique_ptr转移到另一个unique_ptr
- 不能拷贝的例外情况，
  - 可以拷贝和赋值一个将要被销毁的`unique_ptr`
  - 例如：函数返回一个`unique_ptr`；返回一个局部对象的拷贝
  - 这里的拷贝是编译器特殊的拷贝
- 向`unique_ptr`传递删除器
  - 默认情况下，用delete释放它指向的对象
  - 编译时绑定编译器
  - 必须在尖括号中`unique_ptr`指向类型之后提供删除器类型

