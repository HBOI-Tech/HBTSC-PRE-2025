import random

from tuack import dmk
from random import randint

ALPHABET = "abcdefghijklmnopqrstuvwxyz"


class TestCase:
    def __init__(self, n, q):
        self.n = n
        self.q = q
        self.str = ''
        self.queries = []

    def validate(self, n, q):
        # print(n, q, self.n, self.q)
        assert 1 <= self.n <= n and 1 <= self.q <= q
        assert len(self.str) == self.n
        assert len(self.queries) == self.q
        for c in self.str:
            assert c in ALPHABET
        for qr in self.queries:
            l1, r1, l2, r2, k = qr
            # print(l1, r1, l2, r2, k)
            assert 1 <= l1 + k - 1 <= r1 <= self.n
            assert 1 <= l2 + k - 1 <= r2 <= self.n

    def write(self, fin):
        fin.write(f"{self.str}\n{self.q}\n")
        random.shuffle(self.queries)
        for qr in self.queries:
            fin.write(f"{qr[0]} {qr[1]} {qr[2]} {qr[3]} {qr[4]}\n")

    def generate_queries(self, new_q=None, lim_k=None):
        if new_q is not None:
            self.q = new_q
        if lim_k is None:
            lim_k = self.n
        lim_k = min(lim_k, self.n)
        for i in range(self.q - len(self.queries)):
            k = randint(1, lim_k)
            l1 = randint(1, self.n - k + 1)
            r1 = randint(l1 + k - 1, self.n)
            l2 = randint(1, self.n - k + 1)
            r2 = randint(l2 + k - 1, self.n)
            self.queries.append([l1, r1, l2, r2, k])

    def join(self, other):
        self.str += other.str
        self.queries += [[(lambda x: x + self.n)(x) for x in qr[:4]] + [qr[4]] for qr in other.queries]
        self.n += other.n


class RandomTestCase(TestCase):

    def __init__(self, n, q, sigma):
        super().__init__(n, q)
        self.str = ''.join([random.choice(sigma) for _ in range(n)])
        self.generate_queries()


class PlainTestCase(TestCase):

    def __init__(self, n, q, char):
        super().__init__(n, q)
        self.str = char * n
        self.generate_queries()


class DecoratedTestCase(TestCase):
    def __init__(self, n, q, sigma):
        super().__init__(n, q)
        seq = [random.choice(sigma) for _ in range(n)]
        for i in range(n):
            l1 = randint(1, n)
            l2 = randint(1, n)
            k = randint(1, min(n - l1 + 1, n - l2 + 1, 100))
            for j in range(k):
                seq[l2 + j - 1] = seq[l1 + j - 1]
        self.str = ''.join(seq)
        self.generate_queries(lim_k=100)


class ChainTestCase(TestCase):

    def __init__(self, n, q, sigma, length):
        super().__init__(n, q)
        seq = [1, 2, 1]
        cnt = n // length + 1
        for i in range(3, cnt):
            if i % 2 == 1:
                seq.append((i - 3) // 2 + 3)
            else:
                seq.append(seq[i - 3])
        dic = [''.join([random.choice(sigma) for _ in range(length)])]
        for i in range(cnt):
            p = randint(0, length)
            dic.append(dic[i][:p] + ''.join([random.choice(sigma) for _ in range(length - p)]))
        # print(seq, dic)
        current_q = 0
        for s in seq:
            self.str += dic[s - 1]
            if len(self.str) >= self.n:
                break
            if current_q < self.q:
                for i in range(random.randint(1, min(10, self.q - current_q))):
                    current_q += 1
                    l2 = randint(1, len(self.str) - length + 1)
                    r2 = randint(l2 + length - 1, len(self.str))
                    self.queries.append([len(self.str) - length + 1, len(self.str),
                                         l2, r2, random.choice([randint(1,length), length])])
        self.str = self.str[:self.n]
        assert len(self.str) == self.n
        self.generate_queries()


class ComplexTestCase(TestCase):

    def __init__(self, n, q, base):
        super().__init__(n, q)
        self.str = base
        assert len(base) <= n
        current_q = 0
        while len(self.str) < n:
            length = max(1, len(self.str) // randint(1, len(self.str) // 2 + 1))
            l = randint(1, len(self.str) - length + 1)
            r = l + length - 1
            self.str += self.str[l - 1:r]
            # if current_q < q and len(self.str) <= n:
            #     current_q += 1
            #     self.queries.append([
            #         len(self.str) - length + 1, len(self.str),
            #         l, r, randint(1, length)
            #     ])
        self.str = self.str[:n]
        self.generate_queries()


class ComposedTestCase(TestCase):

    def __init__(self, n, q):
        super().__init__(0, q)
        while len(self.str) < n:
            length = randint(1, min(n - len(self.str), n // 8 + 1))
            a_l = randint(0, len(ALPHABET) - 1)
            a_r = randint(a_l + 1, len(ALPHABET))
            sigma = ALPHABET[a_l:a_r]
            type = randint(1, 10)
            # print(self.n, len(self.str), type, length)
            if type <= 2 and self.str:
                l = randint(0, len(self.str) - 1)
                r = randint(l + 1, min(len(self.str), l + length // 10 + 1))
                self.join(ComplexTestCase(length, length // 2, self.str[l:r]))
            elif type <= 7 and length >= 30:
                length = randint(1, min(n - len(self.str), n // 4 + 1))
                self.join(ChainTestCase(length, length, sigma, random.choice([randint(12, 20),
                                                                              randint(1, length // 4 + 1)])))
            elif type <= 8:
                self.join(DecoratedTestCase(length, length // 2, sigma))
            elif type <= 9:
                length = min(length, 20)
                self.join(RandomTestCase(length, length // 2, sigma))
            else:
                self.join(PlainTestCase(length, length // 2, random.choice(sigma)))
            assert (self.n == len(self.str))
        self.generate_queries()
        assert len(self.str) == n


def generate(case, n, q):
    if case == 20:
        tc = TestCase(0, q)
        tc.join(ChainTestCase(n // 2, q // 2, ALPHABET, 15))
        tc.join(ChainTestCase(n // 2, q // 2, ALPHABET, 2000))
        tc.generate_queries()
        return tc
    else:
        tc = ComposedTestCase(n, q)
    return tc


def gen(fin, args, cont):
    dmk.log.debug(f"当前数据类型：{cont['folder']}")

    def get_actual_value(constraint: str):
        val = constraint[4:]
        return eval(val.replace('^', '**').replace('\\times','*'))

    n = get_actual_value(args['n'])
    q = get_actual_value(args['q'])
    spe = args['spe']
    print(f"n = {n}, q = {q}, spe = {spe}")
    case = cont['case']

    test_case = generate(case, n, q)

    if spe == "$l_1 \\le l_2$":
        print("spe A")
        for qr in test_case.queries:
            l1, r1, l2, r2, k = qr
            if l1 > l2:
                qr[0], qr[1], qr[2], qr[3] = l2, r2, l1, r1

    if spe == "$k \\le 10$":
        print("spe B")
        for qr in test_case.queries:
            l1, r1, l2, r2, k = qr
            if k > 10:
                qr[4] = randint(1, 10)

    test_case.validate(n, q)
    test_case.write(fin)


if __name__ == '__main__':
    tc = ComplexTestCase(100, 20, 'test')
    print(tc.str)
    with open("data/test.in", "w") as fin:
        tc.write(fin)
