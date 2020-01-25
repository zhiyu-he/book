### KNN

> knn是一种聚类的算法, 其原理为在给定`(input, labeled_data, label)`的信息下, 以top-k(欧式距离最小)中出现次数最多的label, 作为input的label


#### 优点
1. 精度高、对异常值不敏感、无数据假设

#### 缺点
1. 计算复杂度高
2. 无法给出数据的内在含义？

#### 适用范围
1. 数值型
2. 标称型

#### 问题
1. 单纯提高k, 不一定能提升准确率


#### 延伸阅读
1. [归一化Normalization](https://www.codecademy.com/articles/normalization)