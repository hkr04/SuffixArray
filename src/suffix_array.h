#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include <cmath>
typedef unsigned long long ull;
class suffix_array {
private:
    const ull SIZE = (1 << 8) - 1; // 一个字节
    // sa[i]: 排名第 i 的后缀编号
    // id[i]: 类似于 sa[i]
    // rk[i]: 编号为 i 的后缀排名
    std::string s;
    std::vector<ull> sa, oldrk, rk;
    bool cmp(ull x, ull y, ull w);
    bool cmp(const ull &pos, const std::string &t);
    bool cmp(const std::string &t, const ull &pos);
    ull lower_bound(const std::string &t);
    ull upper_bound(const std::string &t);
    static ull utf8_get_len(const unsigned char &byte);
public:
    suffix_array();
    suffix_array(std::string s); 
    ull size();
    ull get_id(ull suf_rank); // 1-index
    std::string get_suf(ull suf_rank); // 1-index
    ull get_rank(ull suf_id); // 0-index
    ull get_count(const std::string &t);
    std::vector<std::pair<std::string, double>> get_prob(const std::string &t);
    double get_branch_entropy(const std::string &t);
    double get_mutual_information(const std::string &t);
};