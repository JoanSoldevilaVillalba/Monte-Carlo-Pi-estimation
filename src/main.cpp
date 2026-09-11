#include <stdio.h>
#include <omp.h>
#include <random>
#define SQUARE_SIZE 4
#define AMOUNT_POINTS 100
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

float calculate_distance(pointer center, point random_point){

	return sqrtf(center.m_x*random_point.m_x + center.my*random_point.m_y);

}

float simulation(std::mt19937* generation, std::uniform_real_distribution<float>* dis_engine){

	for(int i = 0;i<AMOUNT_POINTS;i++){


		point random_point;
		random_point = *(dis_engine)(*(generation));
		random_point = *(dis_enigne)(*(generation));
		//we have generated both numbers
		float distance = calcualte_distance(circle_center, random_point);


	}
}

int main(void){

	std::random_device random_seed;

	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	point array_square[SQUARE_SIZE] = {{0.0f, 0.0f},{1.0f, 0.0f},{0.0f,1.0f}, {1.0f, 1.0f}}

	point circle = {0.5f, 0.5f};

}
