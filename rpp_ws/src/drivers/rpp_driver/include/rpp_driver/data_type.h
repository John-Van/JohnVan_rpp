#ifndef __H_DATA_TYPE_H__
#define __H_DATA_TYPE_H__

namespace rpp
{

struct DeviceState
{
    uint8_t control_mode; // 0 - idle,1 - APP, 2 - joystick
    uint8_t power_mode;
    uint8_t motor_mode;
    uint8_t e_stop;
    int64_t ts; // time stamp in nanoseconds, since epoch

    DeviceState() : control_mode(0), power_mode(0), motor_mode(0), e_stop(0), ts(0)
    {
    }
};

struct BatteryState
{
    uint8_t percentage;
    float temperature;
    float voltage; // The battery voltage in [V].
    float current; // The battery current in [A].
    int64_t ts;    // time stamp in nanoseconds, since epoch

    BatteryState() : percentage(0), temperature(0), voltage(0), current(0), ts(0)
    {
    }
};

struct LightState
{
    uint8_t front_light;
    uint8_t rear_light;
    int64_t ts; // time stamp in nanoseconds, since epoch
    LightState() : front_light(0), rear_light(0), ts(0)
    {
    }
};

struct ErrorState
{
    uint8_t motor_error;
    uint8_t turn_motor_error;
    uint8_t bump_error;
    uint8_t communication_error;
    int64_t ts; // time stamp in nanoseconds, since epoch
    ErrorState() : motor_error(0), turn_motor_error(0), bump_error(0), communication_error(0), ts(0)
    {
    }
};

struct SingleMotorState
{
    float theta;
    float omega;
    float voltage;
    float current;
    float temperature;
    float rpm;
    int32_t tick;
    uint32_t fault;
    SingleMotorState() : theta(0), omega(0), voltage(0), current(0), temperature(0), rpm(0), tick(0), fault(0)
    {
    }
};

struct MotorState
{
    SingleMotorState motor_states[4];
    int64_t ts; // time stamp in nanoseconds, since epoch
    MotorState() : ts(0)
    {
    }
};

struct Odometry
{
    double x;
    double y;
    double theta;
    int64_t ts; // time stamp in nanoseconds, since epoch
    Odometry() : x(0), y(0), theta(0), ts(0)
    {
    }
};

struct Twist
{
    double x;
    double y;
    double theta;
    int64_t ts; // time stamp in nanoseconds, since epoch
    Twist() : x(0), y(0), theta(0), ts(0)
    {
    }
};

} // namespace rpp

#endif // __H_DATA_TYPE_H__