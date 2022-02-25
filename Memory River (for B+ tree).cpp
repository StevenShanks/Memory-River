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

    //������n��int��ֵ����tmp��1_base
    void get_info(int& tmp, int n) {
        if (n > info_len) return;
        file.open(file_name, std::ios::in);
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int));
        file.close();
    }

    //��tmpд���n��int��λ�ã�1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.open(file_name);
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
        file.close();
    }

    //���ļ�����λ��д�������t��������д���λ������index
    //λ��������ζ�ŵ�������ȷ��λ������index�����������������ж���˳�����ҵ�Ŀ�������в���
    //λ������index����ȡΪ����д�����ʼλ��
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

    //��t��ֵ����λ������index��Ӧ�Ķ��󣬱�֤���õ�index������write��������
    void update(T& t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
    }

    //����λ������index��Ӧ��T�����ֵ����ֵ��t����֤���õ�index������write��������
    void read(T& t, const int index) {
        file.open(file_name, std::ios::in);
        file.seekg(index);
        file.read(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
    }

    //ɾ��λ������index��Ӧ�Ķ��󣬱�֤���õ�index������write��������
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