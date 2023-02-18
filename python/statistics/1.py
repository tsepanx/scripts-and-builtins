def c(n, k):
    return int(math.factorial(n) / (math.factorial(n - k) * math.factorial(k)))
