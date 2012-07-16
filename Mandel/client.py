#!/usr/bin/python

import os
import sys
import subprocess
import urllib2
import tempfile


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
        fh, myfile = tempfile.mkstemp()
        os.close(fh)
        cmdline = ["./mandel"] + args + [myfile]
        subprocess.check_call(cmdline)
        subprocess.check_call(["curl", base+"put/"+task, 
                               "-F", "data=@%s" % myfile, "-k"])
        os.unlink(myfile)
        print

if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))
