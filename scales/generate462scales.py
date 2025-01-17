# 462 scales / Mårten Nettelbladt / 2025-01-14
# Finds all combinations of 7 positive integers that add upp to 12

# range is 1-6
for a in range(1, 7):
    for b in range(1, 7):
        for c in range(1, 7):
            for d in range(1, 7):
                for e in range(1, 7):
                    for f in range(1, 7):
                        for g in range(1, 7):
                            if a + b + c + d + e + f + g == 12:
                                print("{", a, ", ", b, ", ", c, ", ", d, ", ", e, ", ", f, ", ", g, "}", ",", sep='')
