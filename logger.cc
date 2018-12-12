#include <stdio.h>
#include <unistd.h>
#include <sys/uio.h>

#include <iostream>

#include "modbus.h"
#include "influxdb.hpp"

int main(void)
{
    uint16_t tab_reg[32];

    modbus_t* mb = modbus_new_tcp("192.168.0.4", 502);
    modbus_connect(mb);

    modbus_read_registers(mb, 1080, 1, tab_reg);
    int dwh_temperature = tab_reg[0];

    modbus_read_registers(mb, 1090, 4, tab_reg);
    int operation_state = tab_reg[0];
    int outdoor_ambient_temperature = (tab_reg[1] < 32767 ? tab_reg[1] : tab_reg[1] - 65536);
    int water_inlet_unit_temperature = tab_reg[2]; 
    int water_outlet_unit_temperature = tab_reg[3]; 

    influxdb_cpp::server_info si("127.0.0.1", 8086, "yutaki");

    std::string resp;
    int ret = influxdb_cpp::builder()
        .meas("Temperature")
        .field("dwh_temperature", dwh_temperature)
        .field("operation_state", operation_state)
        .field("outdoor_ambient_temperature", outdoor_ambient_temperature)
        .field("water_inlet_unit_temperature", water_inlet_unit_temperature)
        .field("water_outlet_unit_temperature", water_outlet_unit_temperature)
        .post_http(si, &resp);

    modbus_read_registers(mb, 1212, 10, tab_reg);
    int invert_freq    = tab_reg[0];
    int comp_current   = tab_reg[2];
    int defrosting     = tab_reg[5];
    int water_temp_set = tab_reg[7];
    int water_flow_level = tab_reg[8];
    int water_pump_speed = tab_reg[9];

    float capacity = (water_outlet_unit_temperature - water_inlet_unit_temperature)*4.2*water_flow_level/3.6;

    ret = influxdb_cpp::builder()
        .meas("Performance")
        .field("invert_freq", invert_freq)
        .field("comp_current", comp_current)
        .field("defrosting", defrosting)
        .field("water_temp_set", water_temp_set)
        .field("water_flow_level", water_flow_level)
        .field("water_pump_speed", water_pump_speed)
        .field("capacity", capacity)
        .post_http(si, &resp);

  modbus_close(mb);
  modbus_free(mb);
}
