import helpers
import ximu3


def timestamp_format(timestamp):
    return "{:8.0f}".format(timestamp) + " us"


def int_format(value):
    return " " + "{:8.0f}".format(value)


def float_format(value):
    return " " + "{:8.3f}".format(value)


def string_format(string):
    return " \"" + string + "\""


def decode_error_callback(decode_error):
    print(decode_error)


def statistics_callback(statistics):
    print(timestamp_format(statistics.timestamp) +
          int_format(statistics.data_total) + " bytes" +
          int_format(statistics.data_rate) + " bytes/s" +
          int_format(statistics.message_total) + " messages" +
          int_format(statistics.message_rate) + " messages/s" +
          int_format(statistics.error_total) + " errors" +
          int_format(statistics.error_rate) + " errors/s")
    # print(statistics.to_string())  # alternative to above


def inertial_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.gyroscope_x) + " deg/s" +
          float_format(message.gyroscope_y) + " deg/s" +
          float_format(message.gyroscope_z) + " deg/s" +
          float_format(message.accelerometer_x) + " g" +
          float_format(message.accelerometer_y) + " g" +
          float_format(message.accelerometer_z) + " g")
    # print(message.to_string())  # alternative to above


def magnetometer_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.x_axis) + " a.u." +
          float_format(message.y_axis) + " a.u." +
          float_format(message.z_axis) + " a.u.")
    # print(message.to_string())  # alternative to above


def quaternion_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.w_element) +
          float_format(message.x_element) +
          float_format(message.y_element) +
          float_format(message.z_element))
    # print(message.to_string())  # alternative to above


def rotation_matrix_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.xx_element) +
          float_format(message.xy_element) +
          float_format(message.xz_element) +
          float_format(message.yx_element) +
          float_format(message.yy_element) +
          float_format(message.yz_element) +
          float_format(message.zx_element) +
          float_format(message.zy_element) +
          float_format(message.zz_element))
    # print(message.to_string())  # alternative to above


def euler_angles_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.roll) + " deg",
          float_format(message.pitch) + " deg",
          float_format(message.yaw) + " deg")
    # print(message.to_string())  # alternative to above


def linear_acceleration_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.w_element) +
          float_format(message.x_element) +
          float_format(message.y_element) +
          float_format(message.z_element) +
          float_format(message.x_axis) + " g" +
          float_format(message.y_axis) + " g" +
          float_format(message.z_axis) + " g")
    # print(message.to_string())  # alternative to above


def earth_acceleration_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.w_element) +
          float_format(message.x_element) +
          float_format(message.y_element) +
          float_format(message.z_element) +
          float_format(message.x_axis) + " g" +
          float_format(message.y_axis) + " g" +
          float_format(message.z_axis) + " g")
    # print(message.to_string())  # alternative to above


def high_g_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.x_axis) + " g" +
          float_format(message.y_axis) + " g" +
          float_format(message.z_axis) + " g")
    # print(message.to_string())  # alternative to above


def temperature_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.temperature) + " degC")
    # print(message.to_string())  # alternative to above


def battery_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.percentage) + " %" +
          float_format(message.voltage) + " V" +
          float_format(message.charging_status))
    # print(message.to_string())  # alternative to above


def rssi_callback(message):
    print(timestamp_format(message.timestamp) +
          float_format(message.percentage) + " %" +
          float_format(message.power) + " dBm")
    # print(message.to_string())  # alternative to above


def serial_callback(message):
    print(timestamp_format(message.timestamp) + string_format(message.string))
    # print(message.to_string())  # alternative to above


def notification_callback(message):
    print(timestamp_format(message.timestamp) + string_format(message.string))
    # print(message.to_string())  # alternative to above


def error_callback(message):
    print(timestamp_format(message.timestamp) + string_format(message.string))
    # print(message.to_string())  # alternative to above


def run(connection_info):
    connection = ximu3.Connection(connection_info)

    connection.add_decode_error_callback(decode_error_callback)
    connection.add_statistics_callback(statistics_callback)

    if helpers.yes_or_no("Print data messages?"):
        connection.add_inertial_callback(inertial_callback)
        connection.add_magnetometer_callback(magnetometer_callback)
        connection.add_quaternion_callback(quaternion_callback)
        connection.add_rotation_matrix_callback(rotation_matrix_callback)
        connection.add_euler_angles_callback(euler_angles_callback)
        connection.add_linear_acceleration_callback(linear_acceleration_callback)
        connection.add_earth_acceleration_callback(earth_acceleration_callback)
        connection.add_high_g_callback(high_g_callback)
        connection.add_temperature_callback(temperature_callback)
        connection.add_battery_callback(battery_callback)
        connection.add_rssi_callback(rssi_callback)
        connection.add_serial_callback(serial_callback)
        connection.add_notification_callback(notification_callback)
        connection.add_error_callback(error_callback)

    print("Connecting to " + connection.get_info().to_string())

    if connection.open() != "Ok":
        raise Exception("Unable to open connection")

    print("Connection successful")

    connection.send_commands(['{"strobe":null}'], 2, 500)  # send command to strobe LED

    helpers.wait(-1)

    connection.close()
