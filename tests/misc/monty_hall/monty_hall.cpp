#include <iostream>
#include <algorithm>
#include <random>

// Number of tests to run
const int tests = 1000000;
// Number of doors in the game
const int num_doors = 3;

int main() {
    // Array representing the doors: one door has a prize (1) and two doors have no prize (0)
    bool door[] = {1, 0, 0};
    int our_door;          // Variable to track which door we will choose
    int win_count = 0;    // Counter to track the number of wins

    // Random device and engine for shuffling the doors
    std::random_device rd;
    std::default_random_engine rng(rd());

    // Run the simulation for the specified number of tests
    for (int i = 0; i < tests; i++) {
        // Shuffle the doors randomly
        std::shuffle(door, door + num_doors, rng);
        our_door = 0;

        // Select a door based on the shuffled positions of the doors
        // If door 1 has no prize, select door 2
        if (door[1] == 0) 
            our_door = 2;
        // If door 2 has no prize, select door 1
        else if (door[2] == 0)
            our_door = 1;

        // Check if the chosen door has the prize and increment win_count if so
        if (door[our_door] == 1)
            win_count++;
    }

    // Output the total number of wins after all tests
    std::cout << win_count;

    return 0;
}
