//rosrun baxter_hmc baxter_hmc_node
//rostopic pub /newTopic std_msgs/String "test
//rostopic echo /bxpoint_d


#include "ros/ros.h"
#include "math.h"
#include "string.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"

ros::Publisher pub;
std::string point;


void baxterCallback(const std_msgs::String::ConstPtr& msg1)
{
	//Print the gesture's information from the HMP_detector
	point = msg1->data.c_str();
	ROS_INFO("Baxter subscriber point @[%s]",point.c_str());

	//Send the information to the code that moves the Baxter
	// -1 means wrong command
	std_msgs::Int32 num;

	if(point == "left"){
		num.data = 0;
        
	}
	else if(point == "right"){

		num.data = 1;
        
	}
	else if(point == "up"){
		num.data = 2;
        
	}
	else if(point == "down"){

		num.data = 3;
        
	}
	else{	
		num.data = -1;

	}

	pub.publish(num);
}

int main (int argc, char **argv)
{
//Initialized the node, setup the NodeHandle
//for handling the communication with the ROS system
ros::init(argc, argv, "hmc_subscriber");
ros::NodeHandle nh;

//Define the subscriber to person's gesture
ros::Subscriber sub = nh.subscribe
("/swgesture", 1, baxterCallback);

//Define the publisher to send the desired robot's gesture
pub = nh.advertise<std_msgs::Int32> ("/bxpoint_d", 1);
ros::spin();

return 0;
}

