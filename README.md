# Go-Chase-It
Go Chase It project from Udacity Nanodegree "Robotic Software Engineer"

This project serves as an introduction to ROS implementation. A robot model and world was created through Gazebo. This robot would navigate 
the created world by identifying a white ball and following it autonomously. The robot uses laser and camera modifications to search for 
white pixels. Splitting the surveyed vision into three sections (left, middle, right), the robot chooses to move in the direction of the 
most white pixels. When there are no pixels in the field of sight, the robot halts.
