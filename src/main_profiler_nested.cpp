#include <stdio.h>
#include <omp.h>
#include <random>
#include <cstring>
#define SQUARE_SIZE 4
#define AMOUNT_POINTS 1000000
#define NUM_THREADS_SIMULATION 2
#define NUM_THREADS_CALCULATION 3
#define CHUNK_SIMULATION 50
#define CHUNK_CALCULATION 333333
#define NUMBER_OF_SIMULATIONS 100
typedef enum{
BOTTOM_LEFT = 0,
BOTTOM_RIGHT = 1,
TOP_LEFT = 2,
TOP_RIGHT = 3
}square_points;

typedef struct{

        float m_x;

        float m_y;

}point;

inline float calculate_distance(point center, point random_point){

        float difference_x =center.m_x - random_point.m_x;
        float difference_y = center.m_y - random_point.m_y;

        return sqrtf(difference_x*difference_x + difference_y*difference_y);

}
struct thread_profiler{

float number_hits;
float number_total;

};


struct thread_profiler profiler = {0.0f,0.0f};
#pragma omp threadprivate(profiler)

//remmebr that defining this means that each thread is going to have its own personal profiler, all threads of openMP have now autmicaly in compile time allocated  this to their stack, or rather their data stack: .tdata or .tbss
float simulation_array[NUM_THREADS_SIMULATION];//for now we are 
float simulation(point circle_center, std::random_device& random_seed){

        float accumulation_inside = 0.0f;

        float accumulation_total = 0.0f;

        #pragma omp parallel num_threads(NUM_THREADS_CALCULATION) reduction(+:accumulation_total,accumulation_inside)
        {

                int thread_id = omp_get_thread_num();

                std::mt19937 generation(random_seed());//random seed is set, 
                std::uniform_real_distribution<float> dis_engine(0.0f, 1.0f);

                #pragma omp for schedule(static, CHUNK_CALCULATION)
                        for(int i = 0;i<AMOUNT_POINTS;i++){


                                point random_point;

                                random_point.m_x = dis_engine(generation);

                                random_point.m_y = dis_engine(generation);

                                float distance = calculate_distance(circle_center, random_point);

                                float difference = distance - 0.5f;

                                int clover = 0;

                                memcpy(&clover, &difference, sizeof(clover));

                                clover = (int(clover) >>31) & 1;

                                profiler.number_hits = profiler.number_hits + float(clover);

                                profiler.number_total++;

                }

                printf("This is thread with id: %d\n", thread_id);
                printf("Number of total random points generated: %f\n", profiler.number_total);
                printf("Number of total random points that are inside of the circle: %f\n", profiler.number_hits);
                printf("Probability is the following number: %f\n",profiler.number_hits/profiler.number_total);
                printf("The pi estmiate for this thread is the following: %f\n", 4.0f*profiler.number_hits/profiler.number_total);
                printf("Thread with id %d is quitting, goodye\n", thread_id);
                accumulation_inside = profiler.number_hits;
                accumulation_total = profiler.number_total;
                profiler.number_hits = 0.0f;
                profiler.number_total = 0.0f;

        }

	return 4.0f * accumulation_inside/accumulation_total;
}

int main(void){

        std::random_device random_seed;

        point circle_center = {0.5f, 0.5f};

        //we are going to call simulation several times
	float sim_acc=0.0f;
	float incrementer = 0.0f;
        #pragma omp parallel num_threads(NUM_THREADS_SIMULATION) reduction(+:sim_acc,incrementer)
        {

                int thread_id = omp_get_thread_num();

                #pragma omp for schedule(static, CHUNK_SIMULATION)
                for(int i = 0;i<NUMBER_OF_SIMULATIONS;i++){

                        sim_acc = sim_acc + simulation(circle_center, random_seed);
			incrementer++;
                }
		sim_acc = sim_acc / incrementer;//we are taking the mean and then comparing each thread wiuth their reslt
                simulation_array[thread_id] = sim_acc;
		printf("Simulation thread with id %d has the following aproximation: %f\n", thread_id, sim_acc);
		printf("Number of simulations done: %f\n\n",incrementer);

        }

	printf("\n\n----- Main Thread profiler -----\n\n");
	for(int i = 0;i<NUM_THREADS_SIMULATION;i++){

		printf("thread with id %d calculated %f for its pi estimation\n",i, simulation_array[i]);

	}

	printf("Final resolution of pi: %f\n", sim_acc/2.0f);

}

