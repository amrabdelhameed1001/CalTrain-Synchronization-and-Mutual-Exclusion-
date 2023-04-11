#include <pthread.h>
#include "caltrain.h"


void
station_init(struct station *station)
{
    // Initialize station data
    station->waiting_passengers = 0;
    station->empty_seats = 0;
    station->boarded_passengers = 0;

    // Initialize locks and condition variables
    pthread_mutex_init(&station->lock, NULL);
    pthread_cond_init(&station->train_arrived, NULL);
    pthread_cond_init(&station->train_full, NULL);
    pthread_cond_init(&station->passenger_seated, NULL);
}

void station_load_train(struct station *station, int count)
{
    // Acquire lock before modifying station data
    pthread_mutex_lock(&station->lock);

    // Set the number of empty seats on the train
    station->empty_seats = count;

    // Keep looping until all empty seats are filled or there are no more waiting passengers
    while (station->empty_seats > 0 && station->waiting_passengers > 0) {
        // Signal all waiting passengers that the train has arrived
        pthread_cond_broadcast(&station->train_arrived);
        // Wait until the train is full or there are no more waiting passengers
        pthread_cond_wait(&station->train_full, &station->lock);
    }

    // Reset empty seats count after train is loaded
    station->empty_seats = 0;

    // Release lock
    pthread_mutex_unlock(&station->lock);

    // Wait until all passengers are seated before returning
    while (station->boarded_passengers < count) {
        pthread_cond_broadcast(&station->passenger_seated);
    }
}


void
station_wait_for_train(struct station *station)
{
    // Acquire lock before modifying station data
    pthread_mutex_lock(&station->lock);

    // Increment the number of waiting passengers
    station->waiting_passengers++;

    // Wait until there are empty seats on the train
    while (station->empty_seats == 0) {
        pthread_cond_wait(&station->train_arrived, &station->lock);
    }

    // Decrement the number of waiting passengers and empty seats
    station->waiting_passengers--;
    station->empty_seats--;

    // Release lock
    pthread_mutex_unlock(&station->lock);

    // Signal the train that a passenger has boarded
    pthread_cond_signal(&station->train_full);

    // Wait until all passengers are seated before returning
    pthread_mutex_lock(&station->lock);
    while (station->boarded_passengers < station->empty_seats) {
        pthread_cond_wait(&station->passenger_seated, &station->lock);
    }
    // Reset boarded passengers count
    station->boarded_passengers = 0;
    // Release lock
    pthread_mutex_unlock(&station->lock);

}

void
station_on_board(struct station *station)
{
    // Acquire lock before modifying station data
    pthread_mutex_lock(&station->lock);

    // Increment the number of boarded passengers
    station->boarded_passengers++;

    // Release lock
    pthread_mutex_unlock(&station->lock);

    // Signal that a passenger has seated
    pthread_cond_signal(&station->passenger_seated);

}

{
    // lock section as i have threads for passangers don't modife in statio data in the same time.
    pthread_mutex_lock(&station->train_lock);
    station->on_board_passangers --;
    station->empty_seat_train --;
    //train waiting till there is no empty seat in or all passangers seated and there is no waiting passanger.
    if (station->empty_seat_train == 0 ||(station->on_board_passangers == 0 &&
        station->waiting_train_passangers == 0))
        pthread_cond_signal(&station->all_passangers_on_board);
    pthread_mutex_unlock(&station->train_lock);
}