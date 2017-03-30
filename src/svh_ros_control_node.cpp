// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!\file
 *
 * \author  Felix Mauch <mauch@fzi.de>
 * \date    2017-2-30
 *
 */
//----------------------------------------------------------------------


// ROS includes
#include <ros/ros.h>

// package includes
#include "SVHRosControlHWInterface.h"

// other includes

int main (int argc, char** argv)
{
  ros::init(argc, argv, "schunk_svh");

  ros::AsyncSpinner spinner(1);
  spinner.start();

  ros::NodeHandle nh;

  // Create the hardware interface
  SVHRosControlHWInterface svh_hw;
  svh_hw.init(nh, nh);

  controller_manager::ControllerManager cm(&svh_hw, nh);

  ros::Time timestamp_now = ros::Time::now();
  ros::Time timestamp_last = ros::Time::now();
  ros::Duration period;
  while (ros::ok())
  {
    // Get current time and elapsed time since last read
    timestamp_now = ros::Time::now();
    period = timestamp_now - timestamp_last;
    timestamp_last = timestamp_now;

    svh_hw.read(ros::Time::now(), period);
    cm.update(ros::Time::now(), period); // TODO: Check if hardware is enabled?
    svh_hw.write(ros::Time::now(), period);
    period.sleep();
  }

  return 0;
}
