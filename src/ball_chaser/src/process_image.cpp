#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float v, float w_z)
 {
    // Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x =  v;
    srv.request.angular_z = w_z;

    client.call(srv);
 }

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    int white_pixel = 255;

    // Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera

    // Number of white pixels in the left, middle, and right part of the image, respectively

   bool turn_left = false;
   bool turn_right = false;
   bool go_straight = false;
   bool now_stop = false;
   bool white_pixel_present = false;
   float s = 0.0;
   for(int pixel=0; pixel < img.height * img.step; pixel += 3)
          {     
      
            if((img.data[pixel] == white_pixel) && (img.data[pixel+1] == white_pixel) && (img.data[pixel+2] == white_pixel))
             { 
                white_pixel_present = true;
             
               if( white_pixel = true)
                  {
                    int s = pixel % img.step;
                    if(s < img.step / 3)
                      {
                        turn_left = true;
                         if(turn_left = true)
                               { 
                                 drive_robot(0.3,1);
                               }  
                      }
                    else if(s < (img.step/3 * 2))
                      {
                        go_straight = true;
                        if(go_straight = true)
                                { 
                                  drive_robot(0.3,0.0);
                                } 
                      } 
                    else
                      {
                        turn_right = true;
                        if(turn_right = true)
                                { 
                                  drive_robot(0.3,-1);
                                } 
                      }
                     break;
                  }
              }
            if(white_pixel_present = false)
                { 
                  now_stop = true;
                  if(now_stop = true)
                   { 
                     drive_robot(0.0,0.0);
                   }  
                }
          }    
          
  }

int main(int argc, char** argv)
  {
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
  }

