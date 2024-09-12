#include <iostream>
void spinFor(int angle, int duration) {
    // Implementation of spinFor function
}

void stop(int duration) {
    // Implementation of stop function
}

void feeder(float user_amount) {
    float cup_amount = 0;
    do {
        spinFor(180, 1);
        stop(0);
        spinFor(180, 1);
        cup_amount += 0.25;
        std::cout << "Cup amount: " << cup_amount << std::endl;
    } while (cup_amount < user_amount);
}

int main() {
    float user_amount = 0;
    std::cout << "How many cups of food do you want to feed your dog?" << std::endl;
    std::cin >> user_amount;
    feeder(user_amount);
    std::cout << "Times motor rotated: " << user_amount * 4 << std::endl;
    return 0;
}

