#include <stdio.h>
#include <omp.h>
#include <random>
#include <cstring>
#define SQUARE_SIZE 4
#define AMOUNT_POINTS 1000
#define NUM_THREADS 5
#define CHUNK 20
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

float simulation(std::mt19937* generation, std::uniform_real_distribution<float>* dis_engine, point circle_center){


	//in this case, we are spawning or taking from the pool of threads of openMP 5 threads, we are staticly asining the workload, meaning no added overhead
	//and in this case each thread is going to first execute 20 units of workload,

	float accumulation_inside = 0.0f;
	float accumulation_total = 0.0f;
	#pragma omp parallel for schedule(static,CHUNK) num_threads(NUM_THREADS) reduction(+:accumulation_total,accumulation_inside)
	for(int i = 0;i<AMOUNT_POINTS;i++){


		point random_point;
		random_point.m_x = (*dis_engine)(*generation);
		random_point.m_y = (*dis_engine)(*generation);
		//we have generated both numbers
		float distance = calculate_distance(circle_center, random_point);
		//now we need to determinae if the point is inside the circle

		float difference =distance - 0.5f;
		int clover = 0;
		memcpy(&clover, &difference, sizeof(clover));
		clover = (int(clover) >>31) & 1; //if i is equal to one, this means that distance is smaller than 0.5f, which means the point is inside of the circle
		//so this means that we can just add it, if its zero, that means that distance was greated
		accumulation_inside = accumulation_inside + float(clover);
		accumulation_total++;

	}

	return 4 * accumulation_inside/accumulation_total;
}

int main(void){

	std::random_device random_seed;

	std::mt19937 gen(random_seed());

	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	point array_square[SQUARE_SIZE] = {{0.0f, 0.0f},{1.0f, 0.0f},{0.0f,1.0f}, {1.0f, 1.0f}};

	point circle_center = {0.5f, 0.5f};

	float result_of_simulation = simulation(&gen, &dis, circle_center);

	printf("We have the following resolution of pi: %f\n", result_of_simulation);

}
