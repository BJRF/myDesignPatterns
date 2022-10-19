class EagerSingleton {
    private:
    public:
        EagerSingleton();
        ~EagerSingleton();
        static EagerSingleton* instance;
        static EagerSingleton* getIntance();
};

//饿汉式初始化即实例化
EagerSingleton* EagerSingleton::instance = new EagerSingleton();

EagerSingleton* EagerSingleton::getIntance() {
    return instance;
}

int main() {
    //调用静态变量
    EagerSingleton* Eager1 = EagerSingleton::instance;
    return 0;
}