import json


def get_qualified_name(cls):
    return "{}.{}".format(cls.__module__, cls.__name__)


class JSONSerializableMeta(type):

    def __init__(cls, name, bases, attrs):
        if not hasattr(cls, "registry"):
            cls.registry = dict()
        else:
            cls.registry[get_qualified_name(cls)] = cls
        return super().__init__(name, bases, attrs)


class JSONSerializable(metaclass=JSONSerializableMeta):

    @classmethod
    def json_serialize(cls, obj):
        raise NotImplementedError

    @classmethod
    def json_deserialize(cls, obj):
        raise NotImplementedError


class EndOfStream(Exception): pass


class JSONSocketProxy:

    def __init__(self, socket):
        self.socket = socket
        self.read_file = socket.makefile("r")
        self.write_file = socket.makefile("w")

    def _read_obj_json(self):
        message = self.read_file.readline()
        if len(message) == 0 or message[-1] != "\n":
            raise EndOfStream
        return message[:-1]

    def _write_obj_json(self, obj_json):
        self.write_file.write(obj_json + "\n")
        self.write_file.flush()

    def read(self):
        obj_json = self._read_obj_json()
        obj = json.loads(obj_json)
        qname = obj[0]
        data = obj[1]
        if qname == "_":
            return data
        elif qname in JSONSerializable.registry:
            return JSONSerializable.registry[qname].json_deserialize(data)

    def write(self, obj):
        if isinstance(obj, JSONSerializable):
            cls = obj.__class__
            obj_json = json.dumps([
                get_qualified_name(cls),
                cls.json_serialize(obj)])
        else:
            obj_json = json.dumps(["_", obj])
        self._write_obj_json(obj_json)

    def close(self):
        self.read_file.close()
        self.write_file.close()
        self.socket.close()

