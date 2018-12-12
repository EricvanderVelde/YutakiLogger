# YutakiLogger
Logging Yutaki heatpump to InfluxDB

Based on https://github.com/erikzw7/yutaki 

Create influxDB
`curl -POST http://localhost:8086/query --data-urlencode "q=CREATE DATABASE yutaki"`

makes use of:
- Influxdb-cpp: https://github.com/orca-zhang/influxdb-cpp.git
- libmodbus: https://libmodbus.org/




