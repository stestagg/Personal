#!/usr/bin/python

import sys
import subprocess
import urllib2


def main(server):
    base = "https://%s/" % server
    while True:
        req = urllib2.urlopen(base + "get")
        resp = req.read()
        if resp == "end":
            return
        task, data = resp.split("\n")
        print "%s " % task,
        sys.stdout.flush()
        args = data.split(" ")
        cmdline = ["./mandel"] + args + ["temp.png", "200", "200"]
        subprocess.check_call(cmdline)
        subprocess.check_call(["curl", base+"put/"+task, 
                               "-F", "data=@temp.png", "-k"])
        print

if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))
