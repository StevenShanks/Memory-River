#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    struct node {
        int pos;
        node* next;
        node* prev;
        node() { next = NULL; }
        node(int Pos, node* N = NULL, node* P = NULL) {
            pos = Pos; next = N; prev = P;
        }
    };
    node* head;
    node* tail;
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        head = new node;
        tail = new node;
        head->next = tail;
        tail->prev = head;
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
        file.close();
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int& tmp, int n) {
        if (n > info_len) return;
        file.open(file_name, std::ios::in);
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int));
        file.close();
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.open(file_name);
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T& t) {
        node* p = head->next;
        int index;
        if (p == tail) {
            file.open(file_name, std::ios::ate | std::ios::out | std::ios::in);
            index = file.tellp();
        }
        else {
            file.open(file_name);
            file.seekp(p->pos);
            index = file.tellp();
            node* q = p->next;
            head->next = q;
            q->prev = head;
            delete p;
        }
        file.write(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
        return index;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T& t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T& t, const int index) {
        file.open(file_name, std::ios::in);
        file.seekg(index);
        file.read(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
    }

    //删除位置索引index对应的对象，保证调用的index都是由write函数产生
    void Delete(int index) {
        T delT;
        node* tmp = tail->prev;
        tail->prev = new node(index, tail, tmp);
        tmp->next = tail->prev;
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char*>(&delT), sizeofT);
        file.close();
    }
};


#endif //BPT_MEMORYRIVER_HPP