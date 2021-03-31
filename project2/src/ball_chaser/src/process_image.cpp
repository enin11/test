#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{

    ROS_INFO_STREAM("Moving towards the ball");

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;


    if (!client.call(srv))
      ROS_ERROR("Failed to call service drive_robot");


    // TODO: Request a service and pass the velocities to it to drive the robot
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255*3;
    bool see_white = true;
    int section = img.width/3;
    int section_left = section;
    int section_middle = section * 2;
    int section_right = section * 3;
    int white_loc;
    int col;
    int row;
    int index;
    int rpixel;
    int gpixel;
    int bpixel;

    int right_count = 0;
    int left_count = 0;
    int middle_count = 0;

	//ROS_INFO("w %1.2i h %1.2i sl %1.2i sm %1.2i sr %1.2i", (int)img.width, (int)img.height, section_left, section_middle, section_right);
	
      for(row = 0; row < img.height; row++) {
        for (col = 0; col < img.width; col++) {
	  index = row * img.step + col * 3;
	  rpixel = img.data[index];
	  gpixel = img.data[index + 1];
	  bpixel = img.data[index + 2];
 
          if ((rpixel + gpixel + bpixel) == white_pixel) {
            //see_white = false;
            //ROS_INFO("data %1.2i %1.2i %1.2i, %1.2i", (int)rpixel, (int)gpixel, (int)bpixel, (int)index);
 
            // ROS_INFO("white %1.2i %1.2i", (int)row, (int)col);

		  if (col <= section_left) {
			left_count++;
		  }
		  else if(col <= section_middle) {
		        middle_count++;
		  }
		  else {
		        right_count++;
		  }
        }
      }
    }
      
      if (left_count > middle_count && left_count > right_count) {
        drive_robot(0, 2); 
      }
      else if (middle_count > left_count && middle_count > right_count) {
        drive_robot(2, 0);
      }
      else if (right_count > left_count && right_count > middle_count) {
        drive_robot(0, -2);
      }
      else {
        drive_robot(0, 0);
      }


ROS_INFO("data %1.2i %1.2i %1.2i", (int)left_count, (int)middle_count, (int)right_count);


}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/drive_bot/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
