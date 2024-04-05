### 倍增构造后缀数组
时间复杂度 $\mathcal{O}(n\log n)$，空间复杂度 $\mathcal{O}(n)$（但处理大数据集时需要注意有常数）。

### 使用场景
该后缀数组需要保证输入为 utf-8 编码，在此基础上支持中文。使用 C++ 编写，并使用 Cython 构建 Python 库，因此支持的语言有 C++ 和 Python。

### 已有函数
- `size()`：返回文本串长度。
- `get_id(suf_rank)`：输入后缀排名（1-index），返回其在文本串中的位置（0-index）。
- `get_suf(suf_rank)`：输入后缀排名（1-index），返回对应后缀。
- `get_rank(suf_id)`：输入后缀左边界位置（0-index），返回对应字典序排名。
- `get_count(pattern)`：输入模式串，返回其在文本串中出现的次数。
- `get_prob(prompt)`：输入一段文本，返回其下一个字的概率分布。
- `get_branch_entropy(prompt)`：输入一段文本，返回其右分支熵。
- `get_mutual_information(text)`：输入一段文本，返回其互信息大小。

### 部署方式
执行
```
python setup.py build_ext --inplace
```
即可在当前目录下生成库文件。可以直接复制粘贴到使用该库的代码同路径下使用。

### 不足
- 没有使用线性的构造算法（先这样吧）
- 空间常数不小，不便用于大数据集
- 没有充足的测试（目前仅用于个人使用，是一种能用就行的状态）