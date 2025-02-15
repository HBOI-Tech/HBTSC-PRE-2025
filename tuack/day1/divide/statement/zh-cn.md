{{ self.title() }}

{{ s('description') }}

小 X 决定建立一个团队，但是现在团队里只有小 X 一个人。

在接下的 $n$ 天里，小 X 每天都会招募到一个人，其中加入的第 $i$ 个人的能力值为 $v_i$，小 X 的能力值为 $v_0=10^{10^{100}}$。

为了更好管理整个团队，小 X 需要将团队分成小组。具体的，假设现在已经有 $k$ 个人加入了团队，小 X 希望找到一个序列 $0=a_0<a_1<\dots <a_m=k+1$，使得对于所有 $0\le i<m-1$，有 $\sum\limits_{j=a_i}^{a_{i+1}-1}v_j>\sum\limits_{j=a_{i+1}}^{a_{i+2}-1}v_j$。

为了让新加入的队员们感受到幸福感，小 X 希望 $a_m$ 尽可能大，在此基础上 $a_{m-1}$ 尽可能大，在此基础上 $a_{m-2}$ 尽可能大……依此类推。也就是让序列 $a_m,a_{m-1},a_{m-2}\dots a_1$ 的字典序尽可能大。

但是小 X 现在正忙于招募队员，所以他希望你来帮他解决这一问题，你只需要告诉他在 $k=1,2\dots n$ 的时候， $\sum\limits_{i=0}^m(i\times a_i)$ 的值即可。


{{ s('input format') }}

{{ self.input_file() }}

输入共两行。

第一行包含一个整数 $n$。

第二行包含 $n$ 个整数，其中第 $i$ 个数为 $v_i$。

{{ s('output format') }}

{{ self.output_file() }}

输出仅一行，包含 $n$ 个以空格隔开的整数，其中第 $i$ 个数表示 $k=i$ 时的答案。

{{ s('sample', 1) }}

{{ self.sample_text() }}


{{ self.title_sample_description() }}


- 对于 $k=1$，有 $m=2$，$a_0=0,a_1=1,a_2=2$，输出 $0\times 0+1\times 1+2\times 2=5$。
- 对于 $k=2$，有 $m=2$，$a_0=0,a_1=2,a_2=3$，输出 $0\times 0+1\times 2+2\times 3=8$。
- 对于 $k=3$，有 $m=3$，$a_0=0,a_1=1,a_2=3,a_3=4$，输出 $0\times 0+1\times 1+2\times 3+3\times 4=19$。
- 对于 $k=4$，有 $m=4$，$a_0=0,a_1=1,a_2=3,a_3=4,a_4=5$，输出 $0\times 0+1\times 1+2\times 3+3\times 4+4\times 5=39$。
- 对于 $k=5$，有 $m=3$，$a_0=0,a_1=3,a_2=5,a_3=6$，输出 $0\times 0+1\times 3+2\times 5+3\times 6=31$。

{{ s('sample', 2) }}

{{ self.sample_file() }}


样例 $2$ 满足测试点 $1\sim 3$ 的限制。

{{ s('sample', 3) }}

{{ self.sample_file() }}


样例 $3$ 满足测试点 $4\sim 6$ 的限制。

{{ s('sample', 4) }}

{{ self.sample_file() }}


样例 $4$ 满足测试点 $7\sim 10$ 的限制。

{{ s('sample', 5) }}

{{ self.sample_file() }}


样例 $5$ 满足测试点 $13\sim 15$ 的限制。

{{ s('subtasks') }}

{{ tbl('data') }}

特殊性质 $\mathrm{A}$：保证 $v_i$ 在数据范围内随机生成。  

对于全部的测试数据满足：$1\le n\le 10^5$，$1\le v_i\le n$。