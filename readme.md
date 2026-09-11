# Monte Carlo Pi Estimation

A simple C++ program that estimates the value of **π (pi)** using the Monte Carlo method and **OpenMP** for parallel computation.

## How It Works

The Monte Carlo method estimates π by randomly generating points inside a square and checking how many of them fall inside a quarter circle.

The estimation is calculated using:

```text
π ≈ 4 × (points inside circle / total points)
```

The more points generated, the more accurate the estimation becomes.

## Technologies

* C++
* OpenMP
* Monte Carlo method


