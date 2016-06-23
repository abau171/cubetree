import json

class JSONSocketProxy:

    def __init__(self, socket):
        self.socket = socket
        self.read_file = socket.makefile("r")
        self.write_file = socket.makefile("w")

    def read(self):
        message = self.read_file.readline().rstrip("\n")
        return json.loads(message)

    def write(self, obj):
        message = json.dumps(obj)
        self.write_file.write(message + "\n")
        self.write_file.flush()

    def close(self):
        self.read_file.close()
        self.write_file.close()
        self.socket.shutdown()
        self.socket.close()

