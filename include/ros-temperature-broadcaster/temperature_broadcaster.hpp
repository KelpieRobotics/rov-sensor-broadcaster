#ifndef TEMPERATURE_BROADCASTER
#define TEMPERATURE_BROADCASTER

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include "ros-temperature-broadcaster/sht30.h"
#include "ros-temperature-broadcaster/ds18b20.h"

class TemperatureBroadcaster : public rclcpp::Node {
public:
    TemperatureBroadcaster();
private:
    void publish_data();

    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr internal_temperature_pub;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr external_temperature_pub;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr internal_humidity_pub;

    rclcpp::TimerBase::SharedPtr timer_;
    

};

#endif