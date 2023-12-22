class ElevatorProtocol:
    def __init__(self, address=None, mac=None):
        # Initialize constants for message parts
        self.HEADER = bytearray([0x7F, 0xF7])
        self.MAC_LENGTH = 6
        self.CHECKSUM_LENGTH = 1

        self.flag = 0x29
        if not address:
            self.addr = bytearray([0x16, 0x27])
        else:
            self.addr = address

        if not mac: 
            self.mac = bytearray([0x11, 0x22, 0x33, 0x44, 0x55, 0x66])
        else:
            self.mac = mac
            
        self.data_fixed = bytearray([0x01, 0x02, 0x03, 0x04,0x05, 0x06, 0x07, 0x08, 0x9, 0x10, 0x11, 0x12])

    def construct_payload(self, address=None, cmd=None, data=None):
        """
        Constructs the payload for the elevator protocol.

        :param addr: 电梯序列号
        :param cmd_resp: Command/Response byte
        :param data: Data bytes (variable length)
        :return: Constructed payload as bytes
        """
        if address is not None:
            self.addr = address


        # Construct the payload
        payload = bytearray()
        payload.append(self.flag)
        payload += self.addr
        payload+=self.mac

        payload_data = bytearray()
        payload_data+=data
        payload_data+=self.data_fixed

        payload_data_length = len(payload_data)

        payload.append(payload_data_length)
        payload.append(cmd)
        payload+=payload_data

        return payload

    def calculate_checksum(self, length, payload):
        """
        Calculates the XOR checksum for the given length and payload.

        :param length: Length byte
        :param payload: The payload for which to calculate the checksum
        :return: Checksum as a single byte
        """
        checksum = length

        for byte in payload:
            checksum ^= byte
        return checksum

    def construct_message(self, address=None, cmd="", data=""):
        
        """
        Constructs the complete message with checksum.

        :param addr: Address byte
        :param mac: MAC address (6 bytes)
        :param cmd_resp: Command/Response byte
        :param data: Data bytes (variable length)
        :return: Complete message as bytes
        """
        payload = self.construct_payload(address, cmd=cmd, data=data)
        
        length = len(payload) # Length includes the checksum byte
        checksum = self.calculate_checksum(length, payload)


        res = bytearray()
        res += self.HEADER
        res += bytearray([length])
        res += payload 
        res += bytearray([checksum])
        return res


    def decoder(self, response):
        # If response is empty, return None
        if not response:
            return None

        # Extract response and data length from the response
        resp = hex(response[13])
        data_len = response[12]

        # If the length of the response doesn't match the expected length, print an error message and return an empty dictionary
        if not data_len + 11 + 4 == len(response):
            print(f"Response length is not correct: {data_len} + 11  + 4  is qual to {len(response)} ? {data_len + 11 + 4 == len(response)}  ")
            return {}

        # Calculate checksum using the calculate_checksum function
        checksum = self.calculate_checksum(response[2], response[3:-1])
        
        # If calculated checksum doesn't match the one in the response, print an error message and return an empty dictionary
        if checksum != response[-1]:
            print(f"Checksum is not correct: {checksum} != {response[-1]}")
            return {}


        # Define a dictionary of lambda functions to decode the response based on data length
        decode_funcs = {
            0x00: lambda: {
                "resp": resp, 
                "addr": [hex(b) for b in response[4:6]], 
                "data_len": data_len,
                "data": {}},
            0x05: lambda: {
                "resp": resp, 
                "addr": [hex(b) for b in response[4:6]], 
                "data_len": data_len,
                "data": {"status": response[13] & 0b11}},
            0x04: lambda: {
                "resp": resp, 
                "addr": [hex(b) for b in response[4:6]], 
                "data_len": data_len,
                "data": {
                    "floor": response[14] if response[14] <= 200 else -(response[14] % 200), 
                    "gate_status": (response[15] >> 6) & 0b11, 
                    "last_direction": (response[15] >> 4) & 0b11, 
                    "enabled": (response[15] >> 2) & 0b11, 
                    "next_direction": (response[15] >> 0) & 0b11}},
            0x10: lambda: {
                "resp": resp,
                "addr": [hex(b) for b in response[4:6]],
                "data_len": data_len,
                "data": {
                    "floor": response[14] if response[14] <= 200 else -(response[14] % 200), 
                    "gate_status": (response[15] >> 6) & 0b11, 
                    "last_direction": (response[15] >> 4) & 0b11, 
                    "enabled": (response[15] >> 2) & 0b11, 
                    "next_direction": (response[15] >> 0) & 0b11,
                    "stop_time": response[19] << 8 + response[18],
                    "door_open_time": int(response[20] * 0.1),
                    "prev_floor": [response[21], response[22], response[23], response[24], response[25]]}}
        }

        # Return the decoded response by calling the appropriate lambda function based on data length
        # If data length is not in the dictionary, return None
        return decode_funcs.get(data_len, lambda: None)()
# Example usage
if __name__ == "__main__":
    protocol = ElevatorProtocol() # Variable length data

    addr = bytearray([0x16, 0x27])
    cmd_resp = 0x60
    user_data = bytearray([0x1])
    message = protocol.construct_message(address=addr, cmd=cmd_resp, data=user_data)
    for i in message:
        print(hex(i), end=" ")
