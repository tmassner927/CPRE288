#ifndef CYBOT_SCAN_H_
#define CYBOT_SCAN_H_


// Scan value
typedef struct{
    float sound_dist;  // Distance from Ping Sensor
    int IR_raw_val;    // Raw ADC value from IR sensro
} cyBOT_Scan_t;


// Initialize cyBot Scan sensors and servo
void cyBOT_init_Scan(void);

// Point sensors to angle, and get a scan value
// Input Parameters:
// angle: Direction to point the Sensors for taking a measurement
// getScan : The location of a declared cyBOT_Scant_t structure
void cyBOT_Scan(int angle, cyBOT_Scan_t* getScan);




////////////////////////////////////////////
//// Calibrate physical CyBOT            ///
////////////////////////////////////////////

// These will be different for each physical CyBOT servo
// where 0 degrees (right), and 180 degrees (left) is located
// These values can be found by running the servo_calibrate function.
// Once you know the values, then you can set them in main().
int right_calibration_value;
int left_calibration_value;


// Severo Calibrate value struct
typedef struct{
    int right;  // Right (0 degree) calibration value
    int left;   // Left (180 degree) calibration value
} cyBOT_SERVRO_cal_t;


// Return a struct containing the right (0 degree), and
// left (180 degree) values that can be used to set
// right_calibration_value, and left_calibration_value
cyBOT_SERVRO_cal_t cyBOT_SERVO_cal(void);



#endif /* CYBOT_SCAN_H_ */
