#include "suffix_array.h"

bool suffix_array::cmp(ull x, ull y, ull w) {
    return oldrk[x] == oldrk[y] && oldrk[x + w] == oldrk[y + w];
}
bool suffix_array::cmp(const ull &pos, const std::string &t) { 
    // 比较 s[pos:] 和 t 的关系，小于返回 true，大于等于返回 false
    for (ull i = 0; i < t.size(); i++) {
        if (pos + i >= s.size())
            return true;
        if (s[pos + i] != t[i])
            return (unsigned char)s[pos + i] < (unsigned char)t[i];
    }
    return false;
}
bool suffix_array::cmp(const std::string &t, const ull &pos) { 
    // 比较 t 和 s[pos:] 的关系，小于返回 true，大于等于返回 false
    for (ull i = 0; i < t.size(); i++) {
        if (pos + i >= s.size())
            return false;
        if (t[i] != s[pos + i])
            return (unsigned char)t[i] < (unsigned char)s[pos + i];
    }
    return false;
}
ull suffix_array::utf8_get_len(const unsigned char &byte) {
    if ((byte & 0x80) == 0x00) { // 单字节, 0xxxxxxx
        return 1;
    } else if ((byte & 0xE0) == 0xC0) { // 双字节, 110xxxxx
        return 2;
    } else if ((byte & 0xF0) == 0xE0) { // 三字节, 1110xxxx
        return 3;
    } else if ((byte & 0xF8) == 0xF0) { // 四字节, 11110xxx
        return 4;
    } else { // byte 不是一个合法的 UTF-8 开始字节
        return 0;
    }
}
suffix_array::suffix_array() {

}
suffix_array::suffix_array(std::string s):s(s) {
    ull n = s.size(), m = SIZE;
    std::vector<ull> cnt(std::max(n, m) + 1), key(n + 1), id(n + 1);
    sa.resize(n + 1), oldrk.resize(2 * n + 1), rk.resize(2 * n + 1);
    // 对长度为 1 的情况进行初始化
    for (ull i = 1; i <= n; i++)
        cnt[rk[i] = (unsigned char)s[i-1]]++;
    for (ull i = 1; i <= m; i++)
        cnt[i] += cnt[i-1];
    for (ull i = n; i >= 1; i--)
        sa[cnt[rk[i]]--] = i;
    ull p = 0; // 本质不同后缀数
    for (ull len = 1; len <= n; len <<= 1, m = p) {
        // 排序长度为 2len 的后缀
        // 第一关键字：rk[i]，第二关键字：rk[i+len]
        // 如果 i + len > n，则均为最小，在第一轮排序中对应 [1, len] 的排名区间
        // 若 sa[i] > len，其对应作为第二关键字的后缀 sa[i]-len 就应该依次排在之后的位置
        p = 0;
        for (ull i = n; i > n - len; i--) // 不超过 len 的部分
            id[++p] = i;
        for (ull i = 1; i <= n; i++)
            if (sa[i] > len)
                id[++p] = sa[i] - len;

        // 对第二关键字的排序
        // 顺便存下对第二关键字进行排序后排名为 i 的后缀的第一关键字
        fill(cnt.begin(), cnt.end(), 0);
        for (ull i = 1; i <= n; i++) 
            cnt[key[i] = rk[id[i]]]++;
        for (ull i = 1; i <= m; i++)
            cnt[i] += cnt[i-1];
        for (ull i = n; i >= 1; i--)
            sa[cnt[key[i]]--] = id[i];

        // 对第一关键字的排序
        copy(rk.begin(), rk.end(), oldrk.begin());
        p = 0;
        for (ull i = 1; i <= n; i++)
            rk[sa[i]] = (cmp(sa[i-1], sa[i], len)? p: ++p);
        if (p == n)
            break;
    }
    // 转换为在所有 utf-8 起始点中的排名
    copy(rk.begin(), rk.end(), oldrk.begin());
    std::vector<ull> real_rk;
    id.clear();
    for (ull i = 0, len = 0; i < n; i += len) {
        len = utf8_get_len(s[i]);
        // std::cout << s.substr(i, len) << std::endl;
        real_rk.push_back(rk[i + 1]);
        id.push_back(i + 1);
    }
    std::sort(real_rk.begin(), real_rk.end());
    rk.resize(id.size());
    sa.resize(id.size() + 1);
    for (ull i = 0; i < id.size(); i++) {
        rk[i] = std::lower_bound(real_rk.begin(), real_rk.end(), oldrk[id[i]])
                - real_rk.begin()
                + 1;
        sa[rk[i]] = id[i] - 1;
    }
}
ull suffix_array::size() {
    return rk.size();
}
ull suffix_array::get_id(ull suf_rank) { // 1-index
    return sa[suf_rank];
}
std::string suffix_array::get_suf(ull suf_rank) { // 1-index
    return s.substr(sa[suf_rank]);
}
ull suffix_array::get_rank(ull suf_id) { // 0-index
    return rk[suf_id];
}
ull suffix_array::lower_bound(const std::string &t) {
    ull l = 1, r = size() + 1; // rank
    while (l < r) {
        ull mid = (l + r) >> 1;
        // std::cout << "\"" << get_suf(mid) << "\"" << "\n";
        if (mid != size() + 1 && cmp(get_id(mid), t))
            l = mid + 1;
        else
            r = mid;
    }
    // std::cout << "\"" << get_suf(l) << "\"" << "\n";
    return l;
}
ull suffix_array::upper_bound(const std::string &t) {
    ull l = 1, r = size() + 1; // rank
    while (l < r) {
        ull mid = (l + r) >> 1;
        if (mid == size() + 1 || cmp(t, get_id(mid)))
            r = mid;
        else
            l = mid + 1;
    }
    return l;
}
ull suffix_array::get_count(const std::string &t) {
    return upper_bound(t) - lower_bound(t);
}
std::vector<std::pair<std::string, double>> suffix_array::get_prob(const std::string &t) {
    std::vector<std::pair<std::string, double>> prob;
    ull l = lower_bound(t), r = upper_bound(t); // rank [l, r)
    ull p = l;
    if (get_id(p) == s.size() - t.size()) // 跳过后缀为 t 的情况
        p++;
    // 使用倍增法
    while (p < r) {
        ull L = p, R = r; 
        ull len = utf8_get_len(s[get_id(p) + t.size()]); // 确保截断的下一 token 为合法 utf8 字符
        std::string sub = s.substr(get_id(p), t.size() + len);
        // std::cout << sub << std::endl;
        while (L < R) { // L 为第一个后缀大于当前子串的 rank
            ull mid = (L + R) >> 1;
            if (mid == r || cmp(sub, get_id(mid))) // 当前子串 < 枚举后缀
                R = mid;
            else // 当前子串 >= 枚举后缀，不够大
                L = mid + 1;
        }
        // sub 可能是空串
        prob.emplace_back(sub.substr(t.size()), 1.0 * (L - p) / (r - l));
        p = L;
    }
    if (prob.empty())
        prob.emplace_back("[UNK]", 0);
    return prob;
}
double suffix_array::get_branch_entropy(const std::string &t) {
    double be = 0;
    auto prob_list = get_prob(t);
    for (auto p: prob_list) {
        double prob = p.second;
        be += -prob * log(prob + 1e-20);
    }
    return be;
}
double suffix_array::get_mutual_information(const std::string &t) {
    if (t.size() == 1)
        return 0;
    ull count = get_count(t);
    if (count == 0)
        return 0;
    ull total = size(); // 字的总数
    double log_count = log(count), log_total = log(total);
    double pmi = 1e60;
    for (ull i = utf8_get_len(t[0]); i < t.size(); i += utf8_get_len(t[i])) {
        pmi = std::min(pmi, log_total + log_count - log(get_count(t.substr(0, i))) - log(get_count(t.substr(i))));
    }
    return pmi;
}