
// 13.4 消息处理实例
class Message {
public:
	Message(const std::string &str = ""):
		contents(str)
	{

	}
	Message(const Message&);
	Message& operator=(const Message&);
	~Message();
	void save(Folder&);
	void remove(Folder&);
	void put_Msg_in_Folders(const std::set<Folder*>&);
	void remove_Msg_from_Folders();
private:
	std::string contents;
	std::set<Folder*> folders;
};

Message::Message(const Message &m):
	contents(m.contents)
	,folders(m.folders)
{
	put_Msg_in_Folders(folders);
}

void Message::put_Msg_in_Folders(const set<Folder*> &rhs)
{
	for(std::set<Folder*>::const_iterator beg = rhs.begin();
		beg != rhs.end(); ++ beg)
	{
		(*beg)->addMsg(this);
	}
}

Message& Message::operator=(const Message &rhs)
{
	if (&rhs != this)
	{
		remove_Msg_from_Folders();
		contents = rhs.contents;
		folders = rhs.folders;
		put_Msg_in_Folders(rhs.folders);
	}
	return *this;
}

void Message::remove_Msg_from_Folders()
{
	for (std::set<Folder*>::const_iterator beg = folders.begin();
			beg != folders.end(); ++ beg)
	{
		(*beg)->remMsg();
	}
}

Message::~Message()
{
	remove_Msg_from_Folders();
}

// 13.5 管理智能指针
class HasPtr {
public:
	HasPtr(int *p, int i): ptr(p), val(i) { }

	int *get_ptr() const { return ptr; }
	int get_int() const { return val; }

	void set_ptr(int *p) { ptr = p; }
	void set_int(int i) { val = i; }

	int get_ptr_val() const { return *ptr; }
	void set_ptr_val(int val) const { *ptr = val; }
private:
	int *ptr;
	int val;
};

// ptr1 和 ptr2的指针类型的成员变量指向的是同一个地址 会出现悬垂指针 
int obj = 0;
HasPtr ptr1(&obj, 42);
HasPtr ptr2(ptr1);

// 使用计数类
// 将这个类的所有成员都设为私有，将HasPtr类设置为友元，使其成员可以访问U_ptr的成员
// 无需担心悬垂指针 但是这个指针指向的值一旦改变 任一
class U_ptr {
	friend class HasPtr;
	int *ip;
	size_t use;
	U_ptr(int *p): ip(p), use(1) {}
	~U_ptr() { delete ip; }
};
// 现在定义一个新的HasPtr类 
class HasPtr {
public:
	HasPtr(int *p, int i):ptr( new U_ptr(p) ) , val(i) } { }  

	// 复制构造函数 HasPtr hPtr1(hPtr);
	// -> 的优先级比 ++ -- 高
	HasPtr(const HasPtr &orig):ptr(orig.ptr),val(orig.val)
	{
		++ ptr->use;
	}

	// 先将右值的计数加1 在将左操作数中的值减 1 
	HasPtr& operator=(const HasPtr&rhs)
	{
		++ rhs.ptr->use;
		if ( -- ptr->use == 0)
			 delete ptr;

		ptr = rhs.ptr;
		val = rhs.val;
		return *this;
	}

	~HasPtr() 
	{
		if (-- ptr->use == 0)
		{
			delete ptr;
		}
	}

	int *get_ptr() const { return  ptr->ip; }
	int get_int() const { return val; }

	void set_ptr(int *p) { ptr->ip = p; }
	void set_val(int i) { val = i; }

	int get_ptr_val() const { return *ptr->ip; }
	void set_ptr_val() const { *ptr->ip = i; }

private:
	U_ptr *ptr;		// 计数类
	int val;	

};

// 处理指针成员的另一个完全不同的方法，就是给指针成员提供值语义
// 复制构造函数不再复制指针，他将重新分配一个新的int对象
// 赋值操作符不需要分配新对象。
class HasPtr
{
public:
	// copy members and increment the use count
	HasPtr(const int &p, int i): ptr(new int(p)), val(i) {}
	HasPtr(const HasPtr &orig): ptr(new int(*orig.ptr)),val(orig.val) {}

	// accessors must change to fetch value from Ptr object
	int get_ptr_val() const { return *ptr; }
	int get_int() { return val; }

	// change the appropriate data member 
	void set_ptr(int *p) { ptr = p; }
	void set_int( int i ) { val = i; }

	// return or change the value pointed to, so ok for const objects
	void *get_ptr() const { return ptr; }
	void set_ptr_val( int p ) const { *ptr = p; }

	// Note: Every HasPtr is guaranteed to point at an actual int; 
	// We know that ptr cannot be a zero pointer
	HasPtr& operator=(const HasPtr&rchs) {
			*ptr = *rchs.ptr;
			val = rhs.val;
			return *this;
	}

	~HasPtr() {
		delete ptr;
	}
private:
	/* data */
	int *ptr;	// points to an int
	int val;
};	



// 十四章 重载操作符与转换

// 不能重载的操作符 :: .* . ?:
// 通过连接其它合法符号可以创建新的操作符
// 用于内置类型的操作符，其含义不能改变
// 除了函数调用操作符 operator() 之外，重载操作符时使用默认实参是非法的。
/* 	作为类成员的重载函数，其形参看起来比操作数数目少 1。作
	为成员函数的操作符有一个隐含的 this 形参，限定为第一个
	操作数。

	操作符定义为非成员函数时，通常必须将它们设置为所操作类的友元
*/

class Sales_item {
	friend std::istream& operator>>(std::istream&,Sales_item&);
	friend std::ostream& operator<<(std::ostream&,const Sales_item&);

public:
	Sales_item& operator+=(const Sales_item&);
};
Sales_item operator+(const Sales_item&,const Sales_item&);
// cour << item1 + item2 << endl; // 这个是隐式调用 operator+
// cout << operator+(item1, item2) << endl; 这两种调用方法都可

/*
// 将结构体作为map的key的时候，需要重载 < 操作符
将要用作关联容器键类型的类应定义 < 操作符。关联容器默认使用键类型
的 < 操作符。即使该类型将只存储在顺序容器中，类通常也应该定义相等（==）
和小于（<）操作符，理由是许多算法假定这个操作符存在。例如 sort 算法使
用 < 操作符，而 find 算法使用 == 操作符。

当一个重载操作符的含义不明显时，给操作取
一个名字更好。对于很少用的操作，使用命名
函数通常也比用操作符更好。如果不是普通操
作，没有必要为简洁而使用操作符
*/
ostream& operator <<(ostream& os,const ClassType &object)
{
	// any special logic to prepare object

	// actual output of member
	os << ".."; // ..

	// return ostream object
	return os;
}
// Sales_item的输出操作符
ostream&
operator<<(ostream& out, const Sales_item& s)
{
	out << s.isbn << "\t" << s.units_sold << "\t"
		<< s.revenue << "\t" << s.avg_price();
	return out;
}

istream& operator>>(istream& in, Sales_item &s)
{
	double price;
	in >> s.isbn >> s.units_sold >> price;
	// check that the inputs succeeded
	if (in)
		s.revenue = s.units_sold * price;
	else
		s = Sales_item(); // input failed: reset object to default state
	return in;
}
Sales_item operator+(const Sales_item& lhs, const Sales_item& rhs)
{
	Sales_item ret(lhs); // copy lhs into a local object that we'll return
	ret += rhs;	// add in the contents of rhs
	return ret;	// return ret by value
}
inline bool
operator==(const Sales_item &lhs, const Sales_item &rhs)
{
	// must be made a friend of Sales_item
	return lhs.unints_sold == rhs.unints_sold &&
		lhs.revenue == rhs.revenue;
}
inline bool
operator!=(const Sales_item &lhs, const Sales_item &rhs)
{
	return !(rhs == lhs); // != defined in terms of operator==
}
/*
	定义了 operator== 的类更容易与标准库一起使用。有些算法，
如 find，默认使用 == 操作符，如果类定义了 ==，则这些算
法可以无须任何特殊处理而用于该类类型。

	关联容器以及某些算法，使用默认 < 操作符。一般而言，关系
操作符，诸如相等操作符，应定义为非成员函数。
*/
string car("Volks");
car = "Studebaker"; // string = const char *
string model;
model = 'T'; // string = char 
// 为了支持这些操作符，string 类包括了如下成员
// 赋值操作符可以重载。
class string {
public:
	string& operator=(const string&);	// s1 = s2
	string& operator=(const char*);		// s1 = "string"
	string& operator=(char);			// s1 = 'c'
};
Sales_item& Sales_item::operator+=(const Sales_item& rhs)
{
	units_sold += rhs.units_sold;
	revenue += rhs.revenue;
	return *this;
}


// 原型下标操作符
/*
	类定义下标操作符时，一般需要定义两个版本：一个为
非 const 成员并返回引用，另一个为 const 成员并返
回 const 引用。
*/
class  Foo {
public:
	int& operator[](const size_t);
	const int& operator[] (const size_t) const;

	Foo(){}
	~ Foo(){}

	/* data */
private:
	vector<int> data;
};
int& Foo::operator[] (const size_t index)
{
	return data[index];
} 
const int& Foo::operator[] (const size_t index) const
{
	return data[index];
}

// 成员访问操作符
// 引用操作符 * 箭头操作符 ->
class ScrPtr	// private class for use by Screen only
{
	friend class ScreenPtr;
	Screen *sp;
	size_t use;
	ScreenPtr(ScreenPtr *p): sp(p), use(1) {}
	~ScreenPtr() { delete sp; }
};
/*
	smart pointer: Users pass to a pointer to a dynamically allocated Screen, which is 
	automatically destroyed when the last ScreenPtr goes away
*/
class ScreenPtr {
public:
	ScreenPtr(ScreenPtr *p): ptr(new ScrPtr(p)) {}
	ScreenPtr(const ScreenPtr &orig):ptr(orig.ptr) {
		++ ptr->use;
	}
	ScreenPtr& operator=(const ScreenPtr&);
	// if use count goes to zero delete the ScrPtr Object
	~ScreenPtr() { if(--ptr->use == 0) delete ptr; }

	// 引用和指针
	Screen &operator*() { return *ptr->sp; }
	Screen *operator->() { return ptr->sp; }

	const Screen &operator*() const { return *ptr->sp; }
	const Screen *operator->() const { return ptr->sp; }
private:
	ScrPtr *ptr;	// points to use-counted ScrPtr class
};

// 自增 / 自减
class CheckedPtr
{
public:
	CheckedPtr(int *b, int *e):beg(b), end(e), cur(b) {}
	~CheckedPtr(){}

	CheckedPtr& operator++();	// prefix operators
	CheckedPtr& operator--();

	CheckedPtr operator++();	// postfix operators
	CheckedPtr operator--();
	/* data */
private:
	int *beg; // pointer to beginning of the array
	int *end; // one past the end of the array	
	int *curr; // current position within the array
};
// prefix : return reference to incremented/decremented object
CheckedPtr& CheckedPtr::operator++()
{
	if (curr == end)
	{
		throw out_of_range("increment past the end of CheckedPtr");
	}
	++ curr;
	return *this;
}
CheckedPtr& CheckedPtr::operator--()
{
	if (curr == beg)
	{
		throw out_of_range("decrement past the begining of CheckedPtr");
	}
	-- curr;
	return *this;
}
// postfix 
CheckedPtr CheckedPtr::operator++(int)
{
	CheckedPtr ret(*this);
	++ *this;

	return ret;
}
CheckedPtr CheckedPtr::operator--(int)
{
	CheckedPtr ret(*this);
	-- *this;

	return ret;
}


// 调用操作符和函数对象
struct absInt {
	int operator()(int val) {
		return val < 0? -val : val;
	}
};
int i = -42;
absInt absObj;
unsigned int ui = absObj(i); // calls absInt::operator(int)

// 15.2 
/*
	Item sold at an undiscounted price derived classes will define various discount strategies
*/
class Item_base {
public:
	Item_base(const std::string &book = "", double sales_price = 0.0):
		isbn(book), price(sales_price) {}

	// return total sales price for a specified number of items
	// derived classed will override and apply different discount algorithms
	std::string book() const {
		return isbn;
	}

	virtual double net_price(std::size_t n) const {
		return n * price;
	}

	virtual ~Item_base() {}

private:
	std::string isbn;
protected:
	double price;
};


