from elevator_protocol import ElevatorProtocol
import threading
import serial
import time
import json
class ElevatorController:
    def __init__(self, port, baudrate=115200, protocol_mac=None):
        self.serial_port = serial.Serial(port, baudrate, timeout=0.5)
        self.lock = threading.Lock()
        self.elevator_protocol = ElevatorProtocol(mac=protocol_mac)

    def send_command(self, command):
        """
        Sends the given command over the serial port and returns the response.

        This method writes the given command to the serial port, waits for a response, and then returns the response. It also prints the command and response for debugging purposes.

        Parameters:
        command (bytearray): The command to be sent.

        Returns:
        bytearray: The response received from the serial port.
        """
        self.serial_port.write(command)
        resp = self.serial_port.read_until()

        # print command and response
        print(f"Sent command: {[hex(b) for b in command]}\n")
        print(f"Received response: {[hex(b) for b in resp]}")
        print()

        return resp

    def protocol_decoder(self, resp):
        """
        Decodes the given response using the elevator protocol.

        This method passes the given response to the `decoder` method of the `elevator_protocol` object, which decodes the response according to the elevator protocol.

        Parameters:
        resp (bytearray): The response to be decoded.

        Returns:
        dict: A dictionary containing the decoded response data.
        """
        return self.elevator_protocol.decoder(resp)




    def create_and_send_command(self, addr, cmd_resp, user_data=bytearray()):
        """
        Constructs a command message and sends it.

        This method uses the elevator protocol to construct a command message with the given address, command/response code, and user data. It then sends the command using the `send_command` method.

        Parameters:
        addr (bytearray): The address to which the command is to be sent.
        cmd_resp (int): The command/response code.
        user_data (bytearray): Additional data to be included in the command. Defaults to an empty bytearray.

        Returns:
        bytearray: The response received after sending the command.
        """
        command = self.elevator_protocol.construct_message(address=addr, cmd=cmd_resp, data=user_data)
        return self.send_command(command)

if __name__ == "__main__":
    
    # Create an instance of ElevatorController
    ele_controller = ElevatorController("/dev/ttyUSB1")

    # Infinite loop to continuously accept user commands
    while True:
        # Display the menu of commands
        print("请选择指令:\n \
              0. 退出\n \
              1. 点亮目标楼层\n \
              2. 查询所在楼层\n \
              3. 延时关门\n \
              4. Ping\n \
              5. 自动开门\n \
              6. 先开锁在开门\n")

        # Accept user input
        user_input = input("请输入指令: ")
        if not user_input or  int(user_input) not in [0,1,2,3,4,5,6]:
            print("输入错误，请重新输入")
            continue
        # Process user input
        user_input = int(user_input)
        if user_input == 1:
            # Command to light up target floor
            user_input = int(input("请输入目标楼层: "))
            if not user_input:
                print("输入错误，请重新输入")
                continue
            response = ele_controller.create_and_send_command(bytearray([0x16, 0x27]), 0x60, bytearray([user_input]))
        elif user_input ==2:
            # Command to query current floor
            response = ele_controller.create_and_send_command(bytearray([0x10, 0x27]), 0x61)
        elif user_input == 3:
            # Command to delay door closing
            user_input = int(input("请输入目标楼层: "))
            if not user_input:
                print("输入错误，请重新输入")
                continue
            response = ele_controller.create_and_send_command(bytearray([0x08, 0x00]), 0x62, bytearray([user_input]))
        elif user_input == 4:
            # Ping command
            response = ele_controller.create_and_send_command(bytearray([0x00, 0x00]), 0x65)
        elif user_input == 5:
            # Command to automatically open the door
            response = ele_controller.create_and_send_command(bytearray([0x2B, 0x00]), 0x66, bytearray([0x05, 0xe8, 0x03,0xe8, 0x03 ]))
        elif user_input == 6:
            # Command to unlock before opening the door
            response = ele_controller.create_and_send_command(bytearray([0x2B, 0x00]), 0x67, bytearray([0xe8, 0x03,0xe8, 0x03 ]))
        elif user_input == 0:
            # Exit the program
            exit(0)

        # Decode and print the response
        decoded_respone = ele_controller.protocol_decoder(response)
        # convert decoded response dict to json format
        json_response = json.dumps(decoded_respone, indent=4)

        print(json_response)