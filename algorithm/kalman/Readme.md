卡尔曼滤波在倒立摆中的应用
===================================
　　对于一个线性高斯系统，我们认为它内部的状态在时刻ｔ是服从于均值为ｎ维向量![pic]( http://latex.codecogs.com/gif.latex?\boldsymbol{\mu_t})，
协方差为n×n维矩阵![pic]( http://latex.codecogs.com/gif.latex?\boldsymbol{\Sigma_t})的高斯分布。使用(1)式描述该系统</br>　　　　　　　　　　　　　　　
![pic]( http://latex.codecogs.com/gif.latex?x_t=\boldsymbol{A}x_{t-1}+\boldsymbol{B}u_t+\boldsymbol{\varepsilon_t}) 　　　　               　　　　　　　　　　　　　　　(1)
</br>其中![pic]( http://latex.codecogs.com/gif.latex?x_t) 是系统在 t 时刻的 n 维状态描述向量，
A 是系统向下一时刻的n×n维状态转移矩阵，![pic]( http://latex.codecogs.com/gif.latex?u_t)是m维控制向量，
B 是n×m维矩阵，![pic]( http://latex.codecogs.com/gif.latex?\varepsilon_t) 是系统在 t 时刻的 n 维噪声向量，是一个高斯随机变量，所有元素均值为0，n×n维协方差矩阵为R 。
</br>![pic]( http://latex.codecogs.com/gif.latex?\boldsymbol{\varepsilon_t}=\begin{pmatrix}\varepsilon_1\\\varepsilon_2\\\vdots\\\varepsilon_n\end{pmatrix})，
![pic]( http://latex.codecogs.com/gif.latex?\boldsymbol{R}=\begin{pmatrix}cov[\varepsilon_1,\varepsilon_1]&cov[\varepsilon_1,\varepsilon_2]&\cdots&cov[\varepsilon_1,\varepsilon_n]\\cov[\varepsilon_2,\varepsilon_1]&cov[\varepsilon_2,\varepsilon_2]&\cdots&cov[\varepsilon_2,\varepsilon_n]\\\vdots&\ddots&&\vdots\\cov[\varepsilon_n,\varepsilon_1]&cov[\varepsilon_n,\varepsilon_2]&\cdots&cov[\varepsilon_n,\varepsilon_n]\end{pmatrix} )，
</br>