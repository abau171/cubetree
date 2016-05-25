import unittest

class FakeTest(unittest.TestCase):
    def setUp(self):
        print("set up!")
    def tearDown(self):
        print("tear down!")
    def test_1(self):
        self.assertEqual(1, 1)
    def test_2(self):
        with self.assertRaises(ZeroDivisionError):
            1 / 0

if __name__ == "__main__":
    unittest.main()

