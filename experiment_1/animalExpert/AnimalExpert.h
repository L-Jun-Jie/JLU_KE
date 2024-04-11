#ifndef ANIMALEXPERT_H
#define ANIMALEXPERT_H

#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;

// 定义 unordered_set 的哈希函数
struct hash_unordered_set {
    size_t operator()(const unordered_set<string>& s) const {
        size_t hash_value = 0;
        for (const auto& element : s) {
            hash_value ^= std::hash<string>()(element) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
        }
        return hash_value;
    }
};

// 定义 unordered_set 的比较函数
struct equal_unordered_set {
    bool operator()(const unordered_set<string>& a, const unordered_set<string>& b) const {
        return a == b;
    }
};

using myset = unordered_set<string>;
using mymap = unordered_map<unordered_set<string>, string, hash_unordered_set, equal_unordered_set>;

class AnimalExpert {
   private:
    myset m_db;                    // 数据基
    mymap m_kb;                    // 知识库
    size_t forward_infer_num = 0;  // 统计正向推理次数
    size_t reverse_infer_num = 0;  // 统计逆向推理次数
    bool succ = false;

   public:
    AnimalExpert(mymap kb) : m_kb(kb) {}

    /* 可以采取一些启发式的方法进行规则选择，此处随机选择一条规则 */
    pair<myset, string> select_rule(mymap rs) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, rs.size() - 1);

        auto it = std::next(rs.begin(), dis(gen));
        return *it;
    }

    /* 正向推理 */
    void forward_infer(string goal) {
        forward_infer_num++;  // 统计正向推理次数
        mymap rs;             // 存储当前可用的推理规则
        assert(!m_kb.empty());
        /* 扫描 DB 和 KB 选取可用规则集 RS*/
        for (auto k : m_kb) {    // 遍历 KB 中的规则
            auto lhs = k.first;  // 规则左部
            bool is_match = true;
            /* 判断当前规则左部的变量是否都出现在数据基中 */
            for (auto d : lhs) {
                if (m_db.find(d) == m_db.end()) {  // 若存在一个规则左侧变量不出现在数据基中，则匹配失败
                    is_match = false;
                    break;
                }
            }
            if (is_match) {  // 向 rs 中添加
                rs[lhs] = k.second;
            }
        }
        assert(!rs.empty());
        if (rs.empty()) {
            std::cout << "forward_infer: Sorry. I can't come to that conclusion !" << std::endl;
            exit(0);
        }

        while (!rs.empty()) {
            auto r = select_rule(rs);

            string rhs = r.second;  // 规则右部
            std::cout << "forward_infer can deduce: " + rhs << std::endl;
            m_db.insert(rhs);  // 更新数据基
            if (rhs == goal) {
                std::cout << "success!" << std::endl;
                succ = true;
                return;
            }
            rs.erase(r.first);
        }
    }

    myset reverse_infer(string goal) {
        reverse_infer_num++;
        myset new_goals;
        mymap rs;
        for (auto k : m_kb) {       // 在 KB 中匹配规则右部
            string rhs = k.second;  // 规则右部
            if (rhs == goal)
                rs[k.first] = rhs;
        }

        if (rs.empty()) {
            std::cout << "reverse_infer: Sorry. I can't come to that conclusion !" << std::endl;
            exit(0);
        }

        while (!rs.empty()) {
            auto r = select_rule(rs);
            auto lhs = r.first;
            std::cout << "reverse_infer can deduce: ";
            for (auto l : lhs) {
                std::cout << l << " ";
            }
            std::cout << std::endl;

            bool is_succ = true;
            for (auto l : lhs) {
                new_goals.insert(l);
                if (m_db.find(l) == m_db.end()) {
                    is_succ = false;
                    break;
                }
            }
            if (is_succ) {
                std::cout << "success!" << std::endl;
                succ = true;
                return new_goals;
            }
            rs.erase(r.first);
        }
        return new_goals;
    }

    void infer(myset facts, string goal) {
        std::cout << "----------" << " infering " << "----------" << std::endl;
        m_db.insert(facts.begin(), facts.end());  // 将事实集加入数据基
        // map<string, string> used_rs;
        myset goals;
        myset new_goals;
        goals.insert(goal);

        while (true) {
            if (succ)
                break;
            for (auto g : goals) {
                forward_infer(g);
            }
            for (auto g : goals) {
                new_goals = reverse_infer(g);
            }
            goals.insert(new_goals.begin(), new_goals.end());
        }
    }

    void display_static() {
        std::cout << "----------" << " statics: " << "----------" << std::endl;
        std::cout << "forward_infer_num: " << forward_infer_num << std::endl;
        std::cout << "reverse_infer_num: " << reverse_infer_num << std::endl;
        // std::cout << "KB:" << std::endl;
        // for (auto k : m_kb) {
        //     for (auto l : k.first) {
        //         std::cout << l << " ";
        //     }
        //     std::cout << "-> " << k.second << std::endl;
        // }
        // std::cout << "DB:" << std::endl;
        // for (auto d : m_db) {
        //     std::cout << d << " ";
        // }
    }
};

#endif