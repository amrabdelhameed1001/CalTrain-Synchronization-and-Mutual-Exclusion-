#include <pthread.h>

struct station {
	// FILL ME IN

	int waiting_passengers;
    int empty_seats;
    int boarded_passengers;
    pthread_mutex_t lock;
    pthread_cond_t train_arrived;
    pthread_cond_t train_full;
    pthread_cond_t passenger_seated;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);

