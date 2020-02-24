#include <memory>
#include <iostream>
using namespace std;
template<typename Derived, typename Base, typename Del>
unique_ptr<Derived, Del>
static_unique_ptr_cast(unique_ptr<Base, Del>&& p)
{
	auto d = static_cast<Derived *>(p.release());
	return unique_ptr<Derived, Del>(d,
		move(p.get_deleter()));
}
template<typename Derived, typename Base, typename Del>
unique_ptr<Derived, Del>
dynamic_unique_ptr_cast(unique_ptr<Base, Del>&& p)
{
	if (Derived *result = dynamic_cast<Derived *>(p.get())) {
		p.release();
		return unique_ptr<Derived, Del>(result,
			move(p.get_deleter()));
	}
	return unique_ptr<Derived, Del>(nullptr, p.get_deleter());
}

struct Base
{
	int a;
	virtual void f() const { cout << "I am base!\n"; }
	virtual ~Base() {}
};

struct Derived : Base
{
	void f() const override
	{
		cout << "I am derived!\n";
	}
	~Derived() {}
};

void testUnique() {

	auto derivedPtr = make_unique<Derived>();
	derivedPtr->f();

	unique_ptr<Base> base = move(derivedPtr);
	base->f();

	auto derived2 = static_unique_ptr_cast<Derived>(move(base));
	derived2->f();

	auto basePtr = make_unique<Derived>();

	unique_ptr<Base> base2 = make_unique<Derived>();
	base2->f();

	auto derived3 = dynamic_unique_ptr_cast<Derived>(move(base2));
	
}
int main() {
	testUnique();
	system("pause");
}