// Simulate a compiled file that will help test processes and execution of files

int main() {
    // Calculate and add all prime numbers up to 100000
    int sum = 0;
    for (int num = 2; num <= 100000; num++) {
        int is_prime = 1;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            sum += num;
        }
    }
    return sum; // Return the sum of primes as exit code
}
