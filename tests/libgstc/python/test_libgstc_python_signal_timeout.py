#!/usr/bin/env python3
import unittest
import threading
import gstc

class TestGstcSignalTimeoutMethods(unittest.TestCase):

    def test_libgstc_python_signal_timeout(self):
        pipeline = "videotestsrc is-live=true ! identity sleep-time=10000000 signal-handoffs=true name=identity ! xvimagesink"
        gstd_client = gstc.client(loglevel='DEBUG')
        self.assertEqual(gstd_client.pipeline_create ("p0", pipeline), 0)
        self.assertEqual(gstd_client.pipeline_play ("p0"), 0)
        self.assertEqual(gstd_client.signal_timeout("p0", "identity", "handoff", "4000000"), 0)
        ret_con = gstd_client.signal_connect("p0", "identity", "handoff")
        self.assertEqual(ret_con['response']['name'], 'handoff')

if __name__ == '__main__':
    unittest.main()

