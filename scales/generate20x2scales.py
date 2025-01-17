# 20x2 scales / Mårten Nettelbladt / 2025-01-17

# range is 1-3
for a in range(1, 4):
    for b in range(1, 4):
        for c in range(1, 4):
            for d in range(1, 4):
                for e in range(1, 4):
                    for f in range(1, 4):
                        for g in range(1, 4):
                            # Sum of all 7 intervals = 12
                            if a + b + c + d + e + f + g == 12:
                                # Pure 5th, sum of first 4 intervals = 7
                                if a + b + c + d == 7:
                                    # no adjacent "1" except 4th / 5th interval (and first / last)
                                    if a + b > 2 and b + c > 2 and c + d > 2 and e + f > 2 and f + g > 2:
                                        # Select only major scales, sum of first 2 intervals = 4 (major 3rd)
                                        if a + b == 4:
                                            # print major / minor scale
                                            print(a, b, c, d, "-", e, f, g, " (", d, c, b, a, "-", g, f, e, ")", sep='')

