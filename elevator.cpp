#include <stdio.h>
#include <stdlib.h>

#define FLOORS 21
#define ELEVATORS 4

typedef struct {
    int current_floor;
    int direction; // 1 for up, -1 for down, 0 for idle
    int target_floor; // add target floor
    int max_capacity;
    int max_weight;
    int (*can_service)(int);
} Elevator;

Elevator elevators[ELEVATORS];

int can_service_all(int floor) {
    return 1;
}

int can_service_odd(int floor) {
    return floor % 2 == 1;
}

int can_service_even(int floor) {
    return floor % 2 == 0;
}

void initialize_elevators() {
    for (int i = 0; i < ELEVATORS; i++) {
        elevators[i].current_floor = 0;
        elevators[i].direction = 0; // initially all elevators are idle
        elevators[i].max_capacity = (i == 3) ? 20 : 10;
        elevators[i].max_weight = (i == 3) ? 2000 : 800;
        if (i == 0 || i == 3) {
            elevators[i].can_service = can_service_all;
        }
        else if (i == 1) {
            elevators[i].can_service = can_service_odd;
        }
        else {
            elevators[i].can_service = can_service_even;
        }
    }
}

void print_elevator_status() {
    printf("%-10s%-10s%-10s\n", "Elevator", "Floor", "Direction");
    for (int i = 0; i < ELEVATORS; i++) {
        printf("%-10d%-10d%-10s\n", i + 1, elevators[i].current_floor == 0 ? -1 : elevators[i].current_floor, elevators[i].direction == 1 ? "Up" : (elevators[i].direction == -1 ? "Down" : "Idle"));
    }
}

void service_request(int request_floor, int num_passengers, int total_weight) {
    if (request_floor < -1 || request_floor > 20) {
        printf("Invalid floor request.\n");
        return;
    }

    if (request_floor == -1) {
        request_floor = 0;
    }

    int best_elevator = -1;
    int min_distance = FLOORS; // initialize with a large value

    for (int i = 0; i < ELEVATORS; i++) {
        if (elevators[i].can_service(request_floor) &&
            num_passengers <= elevators[i].max_capacity &&
            total_weight <= elevators[i].max_weight) {
            int val = request_floor - elevators[i].current_floor;
            int distance = abs(val);
            int potential_direction = (val < 0) ? -1 : ((val > 0) ? 1 : 0); // calculate potential direction

            if (distance < min_distance &&
                ((potential_direction == 1 && elevators[i].current_floor <= request_floor) ||
                    (potential_direction == -1 && elevators[i].current_floor >= request_floor) ||
                    (potential_direction == 0))) {
                best_elevator = i;
                min_distance = distance;
                elevators[i].direction = potential_direction; // update direction here
            }
        }
    }

    if (best_elevator == -1) {
        printf("No elevator can service the request.\n");
        return;
    }

    elevators[best_elevator].target_floor = request_floor; // set target floor
    elevators[best_elevator].direction = (elevators[best_elevator].current_floor < elevators[best_elevator].target_floor) ? 1 : -1; // update direction based on target floor
    elevators[best_elevator].current_floor = request_floor; // set target floor
    printf("Elevator %d is moving towards the request at floor %d\n", best_elevator + 1, request_floor == 0 ? -1 : request_floor);
    print_elevator_status();
}

int main() {
    initialize_elevators();

    // Example usage
    int request_floor, num_passengers, total_weight;
    while (1) {
        printf("Enter request floor, number of passengers, and total weight (enter -2 to quit):\n");
        int ret = scanf_s("%d", &request_floor);
        if (ret != 1) {
            printf("Invalid input. Please enter an integer.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        if (request_floor == -2) {
            break;
        }
        ret = scanf_s("%d %d", &num_passengers, &total_weight);
        if (ret != 2) {
            printf("Invalid input. Please enter two integers.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        service_request(request_floor, num_passengers, total_weight);
    }

    return 0;
}
