#include<mutex>
#include<atomic>
using namespace std;

class LazySingleton
{
private:
    /* data */
    static LazySingleton* instance;
    static atomic<LazySingleton*> atomic_instance;
    static mutex lock;
    //构造函数设置在内部，让外部无法创建实例
    LazySingleton();
    LazySingleton(const LazySingleton& other);
public:
    //LazySingleton();
    static LazySingleton* getInstance();
    static LazySingleton* getInstance2();
    static LazySingleton* getInstance3();
    static LazySingleton* getInstance4();
    ~LazySingleton();
};

//懒汉式,线程不安全
LazySingleton* LazySingleton::getInstance() {
    if(instance == nullptr) {
        instance = new LazySingleton();
    }
    return instance;
}

//懒汉式,线程安全改进版本,但是其实C++11已经对static底层实现了加锁操作，所以也可以不加锁
LazySingleton* LazySingleton::getInstance2() {
    lock.lock();
    if(instance == nullptr) {
        instance = new LazySingleton();
    }
    lock.unlock();
    return instance;
}

/*
懒汉式,双检查锁改进版本,锁前锁后都检查,这样不影响读操作,同时也对写操作加锁了,
但是由于内存读写reorder不安全,有可能先分配了内存但是还没有初始化内容,
这个时候可以使用volatile防止编译器优化
*/
LazySingleton* LazySingleton::getInstance3() {
    if(instance == nullptr) {
        lock.lock();
        if(instance == nullptr) {
            instance = new LazySingleton();
        }
    }
    lock.unlock();
    return instance;
}

//C++11版本的volatile
LazySingleton* LazySingleton::getInstance4() {
    LazySingleton* tmp = atomic_instance.load(memory_order_relaxed);//加载原子操作，内存排序
    atomic_thread_fence(memory_order_acquire);//获取内存fence
    if(tmp == nullptr) {
        lock_guard<mutex> lock(mutex);//lock_guard利用对象生命周期加锁方式
        if(tmp == nullptr) {
            tmp = new LazySingleton();
            atomic_thread_fence(memory_order_release);//释放内存fence
            atomic_instance.store(tmp, memory_order_relaxed);//原子替换tmp指向的原子变量
        }
    }
    lock.unlock();
    return instance;
}
