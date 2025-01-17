# 20 scales / Mårten Nettelbladt / 2025-01-16

for a in range(3):
    for b in range(3):
        for c in range(3):
            for d in range(3):
                for e in range(3):
                    for f in range(3):
                        for g in range(3):
                            if a + b + c + d + e + f + g + 7 == 12:
                                # no adjacent "1" except 4th / 5th interval and first / last. Pure 5th
                                if a + b > 0 and b + c > 0 and c + d > 0 and e + f > 0 and f + g > 0 and a + b + c + d + 4 == 7:
                                    # Select only major scales
                                    if a + b + 2 == 4:
                                        # print major / minor scale
                                        print(a+1, b+1, c+1, d+1, "-", e+1, f+1, g+1, " (", d+1, c+1, b+1, a+1, "-", g+1, f+1, e+1, ")", sep='')

