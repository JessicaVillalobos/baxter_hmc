//rosrun baxter_hmc baxter_hmc_node
//rostopic pub /swgesture baxter_hmc/swgesture "swgesture: 'right'"
//rostopic echo /Num/Num 


#include "ros/ros.h"
#include "math.h"
#include "string.h"
#include "baxter_hmc/swgesture.h"
#include "baxter_hmc/Num.h"

ros::Publisher pub;
std::string point;


void baxterCallback(const baxter_hmc::swgesture msg1)
{
	//Print the gesture's information from the HMP_detector
	point = msg1.swgesture;
	ROS_INFO("Baxter subscriber point @[%s]",point.c_str());

	//Send the information to the code that moves the Baxter
	// -1 means wrong command
	baxter_hmc::Num number;

	if(point == "left"){

		number.Num = 0;
        
	}
	else if(point == "right"){

		number.Num = 1;
        
	}
	else if(point == "up"){

		number.Num = 2;
        
	}
	else if(point == "down"){

		number.Num = 3;
        
	}
	else{
		
		number.Num = -1;

	}

	pub.publish(number);
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
pub = nh.advertise<baxter_hmc::Num> ("/Num/Num", 1);
ros::spin();

return 0;
}
