#include "ros-temperature-broadcaster/temperature_broadcaster.hpp"


TemperatureBroadcaster::TemperatureBroadcaster() : Node("temperature_broadcaster_node")
{
    internal_temperature_pub = this->create_publisher<std_msgs::msg::Float32>("internal_temperature", 10);
    external_temperature_pub = this->create_publisher<std_msgs::msg::Float32>("external_temperature", 10);
    internal_humidity_pub = this->create_publisher<std_msgs::msg::Float32>("internal_humidity", 10);

    timer_ = this->create_wall_timer(std::chrono::seconds(1), std::bind(&TemperatureBroadcaster::publish_data, this));

    initSHT30();

}

void TemperatureBroadcaster::publish_data()
{
    readSHT30();

    auto internal_temp_msg = std_msgs::msg::Float32();
    internal_temp_msg.data = getTemperatureSHT30();
    internal_temperature_pub->publish(internal_temp_msg);

    auto external_temp_msg = std_msgs::msg::Float32();
    external_temp_msg.data = readDS18B20();
    external_temperature_pub->publish(external_temp_msg);


    auto internal_humi_msg = std_msgs::msg::Float32();
    internal_humi_msg.data = getHumiditySHT30();
    internal_humidity_pub->publish(internal_humi_msg);

    RCLCPP_INFO(this->get_logger(), "Published: IT %.2f, ET %.2f, IH %.2f", internal_temp_msg.data, external_temp_msg.data, internal_humi_msg.data);

}

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TemperatureBroadcaster>());
    rclcpp::shutdown();

    return 0;
}
