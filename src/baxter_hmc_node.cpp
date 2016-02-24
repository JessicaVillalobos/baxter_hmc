/** @file */ 
//rosrun baxter_hmc baxter_hmc_node
//rostopic pub /swgesture std_msgs/String "up"
//rostopic echo /bxpoint_d


#include "ros/ros.h"
#include "math.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include <iostream>
#include <string>
#include <ctime>
using std::time;
using std::cout;
using std::cin;
using std::string;

ros::Publisher pub;
std_msgs::Int32 num;
string point;
string scenario = "0";
int loop = 0;
int sendPoint = -1;

//! Ask for a valid scenario option
void scenarioNot(){
	ROS_INFO("Choose a valid option");
	getline(cin,scenario,'\n');
}

//! Give all the options and ask the user to select one
void selectScenario(){

	cout<<"Choose the test you want:\n";
	cout<<"1) Scenario 1 - Without Errors\n";
	cout<<"2) Scenario 2 - Wait until the correct position is reached (three last loops)\n";
	cout<<"3) Scenario 3 - Wait until the correct position is reached (last loop)\n";
	cout<<"4) Scenario 4 - Randomly choosing to point correctly or not\n";
	cout<<"-1) Exit\n";

	getline(cin,scenario,'\n');
	
	//! Check if they want to end the program or if something 
	//! is wrong

	if(scenario == "-1"){
		exit(0);
     	}
	while(scenario != "1" && scenario != "2" && scenario != "3" && scenario != "4" && scenario != "-1"){
		scenarioNot();
	}

}

//! Do not change the points received before sending them
void scenarioOne(){
	ROS_INFO("Baxter publish point @[%s]",point.c_str());
}

//! Change the points received for random positions during the
//! first 3 or 5 loops (scenarios 2 and 3)
int scenarioTwo(int wrong){
	if(loop < wrong){
		sendPoint = rand()%4;
		while(sendPoint == num.data){
			sendPoint = rand()%4;
		}
		// robot's right
		if(sendPoint == 0){
			num.data = 0; 
			point = "penguin";       
		}
		// robot's left
		else if(sendPoint == 1){
			num.data = 1;   
			point = "window";      
		}
		// up
		else if(sendPoint == 2){
			num.data = 2;  
			point = "lamp";       
		}
		// in front of the robot
		else if(sendPoint == 3){
			num.data = 3;
			point = "person"; 
		}
	}
	ROS_INFO("Baxter publish point @[%s]",point.c_str());
}

//! Always send random positions
void scenarioFour(){
	sendPoint = rand()%4;
	if(sendPoint == 0){
		num.data = 0; 
		point = "penguin";       
	}
	else if(sendPoint == 1){
		num.data = 1;   
		point = "window";      
	}
	else if(sendPoint == 2){
		num.data = 2;  
		point = "lamp";       
	}
	else if(sendPoint == 3){
		num.data = 3;
		point = "person"; 
	}
	ROS_INFO("Baxter publish point @[%s]",point.c_str());
}


void baxterCallback(const std_msgs::String::ConstPtr& msg1)
{
	//! Print the gesture's information from the HMP_detector
	point = msg1->data.c_str();
	ROS_INFO("Baxter subscriber point @[%s]",point.c_str());

	//! Send the information to the code that moves the Baxter
	//! -1 means wrong command
	if(point == "penguin"){
		num.data = 0;        
	}
	else if(point == "window"){
		num.data = 1;        
	}
	else if(point == "lamp"){
		num.data = 2;        
	}
	else if(point == "person"){
		num.data = 3;
	}
	else{	
		num.data = -1;
	}

	//! Execute the code of the chosen scenario
	if(loop < 6){
		if(scenario == "1")
			scenarioOne();
		else if(scenario == "2")
			scenarioTwo(3);
		else if(scenario == "3")
			scenarioTwo(5);
		else if(scenario == "4")
			scenarioFour();
		loop = loop + 1;
	}

	pub.publish(num);

	//! Ask the user to select another option after all the
	//! executions of a scenario
	if(loop == 6){
		loop = 0;
		scenario = "0";
		selectScenario();
	}
}

int main (int argc, char **argv)
{

	selectScenario();
	srand(time(0)); 
	
	//! Initialized the node, setup the NodeHandle
	//! for handling the communication with the ROS system
	ros::init(argc, argv, "hmc_sub");
	ros::NodeHandle nh;

	//! Define the subscriber to the person's gesture
	ros::Subscriber sub = nh.subscribe
	("/swgesture", 1, baxterCallback);

	//! Define the publisher to send the desired robot's gesture
	pub = nh.advertise<std_msgs::Int32> ("/bxpoint_d", 1);
	ros::spin();

return 0;
}
