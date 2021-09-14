#ifndef CYBOT_SCAN_H_
#define CYBOT_SCAN_H_


// Scan value
typedef struct{
	float sound_dist;  // Distance from Ping Sensor
	int IR_raw_val;    // Raw ADC value from IR sensro
} cyBOT_Scan_t;


// Initialize cyBot Scan sensors and servo
void cyBOT_init_Scan(void);


// Point sersors to angle, and get a scan value
// Input: int angle, angle to point sensor
// Input: cyBOT_Scan_t* getScan, location of a CyBOT_Scan_t struct
// Note 1: To pass the location of a varble use the & operator.
// For example:
//   int my_angle
//   cyBOT_Scan_t x;
//   cyBOT_Scan(my_angle, &x);
// Note 2: It is OK that this function is not calabrated to 
// 0 - 180 degrees.  You will calibrate your version in Lab 9.
void cyBOT_Scan(int angle, cyBOT_Scan_t* getScan);


#endif /* CYBOT_SCAN_H_ */
