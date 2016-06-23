import queue

class IterableQueue:

    def __init__(self):
        self.iterator = None
        self.queue = queue.Queue()

    def _try_add_next(self):
        if self.iterator is not None:
            try:
                next_item = next(self.iterator)
                self.queue.put(next_item)
            except StopIteration:
                self.iterator = None

    def set_iterable(self, iterable):
        self.iterator = iter(iterable)
        self._try_add_next()

    def get(self):
        self._try_add_next()
        return self.queue.get()

    def task_done(self):
        self.queue.task_done()

    def join(self):
        self.queue.join()

