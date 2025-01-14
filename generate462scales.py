
# 462 scales / Mårten Nettelbladt / 2025-01-14

sum12 = 0

for a in range(6):
    for b in range(6):
        for c in range(6):
            for d in range(6):
                for e in range(6):
                    for f in range(6):
                        for g in range(6):
                            if a + b + c + d + e + f + g + 7 == 12:
                                sum12 += 1
                                print(a+1, ",", b+1, ",", c+1, ",", d+1, ",", e+1, ",", f+1, ",", g+1)
