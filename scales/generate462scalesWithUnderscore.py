# 462 scales / Mårten Nettelbladt / 2025-01-15

for a in range(6):
    for b in range(6):
        for c in range(6):
            for d in range(6):
                for e in range(6):
                    for f in range(6):
                        for g in range(6):
                            if a + b + c + d + e + f + g + 7 == 12:
                                a_ = ''
                                for i in range(a):
                                    a_ += "_"
                                b_ = ''
                                for i in range(b):
                                    b_ += "_"
                                c_ = ''
                                for i in range(c):
                                    c_ += "_"
                                d_ = ''
                                for i in range(d):
                                    d_ += "_"
                                e_ = ''
                                for i in range(e):
                                    e_ += "_"
                                f_ = ''
                                for i in range(f):
                                    f_ += "_"
                                g_ = ''
                                for i in range(g):
                                    g_ += "_"
                                print(a+1, a_, b+1, b_, c+1, c_, d+1, d_, e+1, e_, f+1, f_, g+1, g_, sep='')
