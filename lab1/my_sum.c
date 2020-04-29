/*
 * This program takes in no command-line arguments (if you put some in, they're
 * ignored).
 * It calculates the sum of the first 100 positive integers and prints the result.
 */

#include <stdio.h>

/*
 * Return the sum of the first n positive integers.
 * Return 0 if n <= 0.
 */
int first_n_sum(int n) {
    if (n <= 0) {
        return 0;
    } else {
        int sum = 0;
        for (int i = 1; i <= n; i++) {
            sum += i;
        }
        return sum;
    }
}

int main() {
    int answer = first_n_sum(100);
    printf("%d\n", answer);
    float a = 1.11;
    int d = a;
    return 0;
}
