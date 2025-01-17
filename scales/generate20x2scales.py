# 20x2 scales / Mårten Nettelbladt / 2025-01-17

for a in range(3):
    for b in range(3):
        for c in range(3):
            for d in range(3):
                for e in range(3):
                    for f in range(3):
                        for g in range(3):
                            # Sum of all 7 intervals = 12
                            if a + b + c + d + e + f + g + 7 == 12:
                                # no adjacent "1" except 4th / 5th interval (and first / last)
                                if a + b > 0 and b + c > 0 and c + d > 0 and e + f > 0 and f + g > 0:
                                    # Pure 5th, sum of first 4 intervals = 7
                                    if a + b + c + d + 4 == 7:
                                        # Select only major scales, sum of first 2 intervals = 4
                                        if a + b + 2 == 4:
                                            # print major / minor scale
                                            print(a+1, b+1, c+1, d+1, "-", e+1, f+1, g+1, " (", d+1, c+1, b+1, a+1, "-", g+1, f+1, e+1, ")", sep='')

                                            # +1 added to every note because range is 0-2 rather than 1-3

