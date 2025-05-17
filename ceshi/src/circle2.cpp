#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/SetPen.h>
#include <turtlesim/TeleportAbsolute.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For time()

int main(int argc, char **argv)
{
    ros::init(argc, argv, "draw_rainbow_spiral");
    ros::NodeHandle nh;
    
    // Create a client to spawn a turtle
    ros::service::waitForService("spawn");
    ros::ServiceClient spawnClient = nh.serviceClient<turtlesim::Spawn>("spawn");
    
    turtlesim::Spawn::Request req;
    turtlesim::Spawn::Response resp;
    
    req.x = 5.5;
    req.y = 5.5;
    req.theta = 0;
    req.name = "rainbow_turtle";
    
    spawnClient.call(req, resp);
    
    // Create publisher for turtle commands (not used here, but kept for completeness)
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("rainbow_turtle/cmd_vel", 10);
    
    // Create clients for pen and teleport services
    ros::ServiceClient penClient = nh.serviceClient<turtlesim::SetPen>("rainbow_turtle/set_pen");
    ros::ServiceClient teleportClient = nh.serviceClient<turtlesim::TeleportAbsolute>("rainbow_turtle/teleport_absolute");
    
    // Initialize random seed
    srand(time(0));
    
    // Parameters for the spiral
    const int circles = 10;                  // Number of circles
    const double center_x = 5.5;             // Center X coordinate
    const double center_y = 5.5;             // Center Y coordinate
    const double max_radius = 4.5;           // Starting radius
    const double min_radius = 0.5;           // Minimum radius
    const double radius_step = (max_radius - min_radius) / circles;
    
    // Start drawing from the outer edge
    turtlesim::TeleportAbsolute::Request teleport_req;
    turtlesim::TeleportAbsolute::Response teleport_resp;
    teleport_req.x = center_x + max_radius;
    teleport_req.y = center_y;
    teleport_req.theta = 0;
    teleportClient.call(teleport_req, teleport_resp);
    
    // Lower the pen (initial settings)
    turtlesim::SetPen::Request pen_req;
    turtlesim::SetPen::Response pen_resp;
    pen_req.r = 255;
    pen_req.g = 0;
    pen_req.b = 0;
    pen_req.width = 8;  // Increased initial width (originally 5)
    pen_req.off = 0;
    penClient.call(pen_req, pen_resp);
    
    ros::Rate loop_rate(30);  // Increased loop rate (originally 10) for faster drawing
    
    for (int i = 0; i < circles; ++i)
    {
        double current_radius = max_radius - (i * radius_step);
        double current_width = 8 - (i * 0.6);  // Wider and slower decrease (originally 5 - 0.4*i)
        
        // Generate random color
        pen_req.r = rand() % 256;
        pen_req.g = rand() % 256;
        pen_req.b = rand() % 256;
        pen_req.width = current_width;
        penClient.call(pen_req, pen_resp);
        
        // Draw a circle with fewer steps (for faster drawing)
        int steps = 50;  // Reduced from 100 to speed up
        for (int j = 0; j <= steps; ++j)
        {
            double angle = 2 * M_PI * j / steps;
            double x = center_x + current_radius * cos(angle);
            double y = center_y + current_radius * sin(angle);
            
            teleport_req.x = x;
            teleport_req.y = y;
            teleport_req.theta = angle + M_PI_2;
            teleportClient.call(teleport_req, teleport_resp);
            
            ros::spinOnce();
            loop_rate.sleep();
        }
    }
    
    // Lift the pen at the end
    pen_req.off = 1;
    penClient.call(pen_req, pen_resp);
    
    return 0;
}