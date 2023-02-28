#include <iostream>
#include <string>
using namespace std;

class Toy{
    string name;
public:
    Toy(string name) : name(name) {};
    Toy(): name("Sometoy") {};
};



typedef unsigned int uint;

template<class T>
class shared_ptr_toy
{
private:
    T * ptr = nullptr;
    uint * refCount = nullptr;

public:
    shared_ptr_toy() : ptr(nullptr), refCount(new uint(0)){}

    shared_ptr_toy(T * ptr) : ptr(ptr), refCount(new uint(1)){}


    shared_ptr_toy(const shared_ptr_toy & obj) // copy constructor
    {
        this->ptr = obj.ptr;
        this->refCount = obj.refCount;
        if (nullptr != obj.ptr)
        {
            (*this->refCount)++;
        }
    }

    shared_ptr_toy& operator=(const shared_ptr_toy & obj) // copy assignment
    {
        _cleanup_(); // cleanup any existing data


        this->ptr = obj.ptr;
        this->refCount = obj.refCount;
        if (nullptr != obj.ptr)
        {
            (*this->refCount)++;
        }
    }

    /*** Move Semantics ***/
    shared_ptr_toy(shared_ptr_toy && dyingObj) // move constructor
    {
        this->ptr = dyingObj.ptr; // share the underlying pointer
        this->refCount = dyingObj.refCount;
       // dyingObj.ptr = dyingObj.refCount = nullptr;
    }

    shared_ptr_toy& operator=(shared_ptr_toy && dyingObj) // move assignment
    {
        _cleanup_();

        this->ptr = dyingObj.ptr;
        this->refCount = dyingObj.refCount;

        dyingObj.ptr = dyingObj.refCount = nullptr;
        return *this;
    }

    uint get_count() const
    {
        return *refCount;
    }

    T* get() const
    {
        return this->ptr;
    }

    T* operator->() const
    {
        return this->ptr;
    }

    T& operator*() const
    {
        return this->ptr;
    }

    ~shared_ptr_toy() // destructor
    {
        _cleanup_();
    }

private:
    void _cleanup_()
    {
        (*refCount)--;
        if (*refCount == 0)
        {
            if (nullptr != ptr)
                delete ptr;
            delete refCount;
        }
    }
};

shared_ptr_toy<Toy> make_shared_toy(Toy* toy){
   shared_ptr_toy<Toy> p1(toy);
   return p1;
}

int main() {
    shared_ptr_toy<Toy> toy = make_shared_toy(new Toy("Ball"));
    Toy* f = toy.get();
    cout << toy.get();
    cout << endl;

    cout << f;

    return 0;
}
