#!/usr/bin/python

from flask import Flask, request, redirect, render_template 
import subprocess
from io import StringIO
import json
import gmpy
import os

app = Flask(__name__)

BIN_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "bin"))
MAP_FILE = os.path.join(BIN_DIR, "../", "map.json")

SIZE=200


def check_png(cmd, args, stdin=None):
    full_cmd = os.path.join(BIN_DIR, cmd)
    proc = subprocess.Popen((full_cmd, ) + tuple(args), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate(stdin)
    assert proc.returncode == 0, stderr
    return stdout


def get_map():
    return open("map.json", "r").read()


def get_json():
    return json.loads(get_map())


def set_json(val):
    with open("map.json", "w") as fp:
        return json.dump(val, fp)


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/explore/")
def explore_base():
    return redirect("/explore/0/0/4/50/")


@app.route("/explore/<real>/<imag>/<wid>/<iters>/")
def explore(real, imag, wid, iters):
    iters = int(float(iters))
    return render_template("explore.html", **locals())


@app.route("/map/")
def goto_map():
    return redirect("/map/10/")


@app.route("/map/<int:img_no>/")
def map(img_no=0):
    return render_template("map.html", **locals())


@app.route("/map/bw/<img_no>/")
def bw_img(img_no=0):
    from_name = "%s" % img_no
    base = "/Users/sstagg/tmp/short1small/%s" % from_name    
    bw = check_png("2bw", [base, "/dev/stdout"], stdin=None)
    return bw, 200, {"Content-type": "image/png"}


@app.route("/map/mapped/<img_no>/")
def map_img(img_no=0):
    from_name = "%s" % img_no
    base = "/Users/sstagg/tmp/short1small/%s" % from_name
    bw = check_png("pp", [MAP_FILE, base, "/dev/stdout"], stdin=None)
    return bw, 200, {"Content-type": "image/png"}


@app.route("/explore/<real>/<imag>/<wid>/<iters>/scale/<new_x>/<new_y>/<ratio>/")
def scale_view(real, imag, wid, iters, new_x, new_y, ratio):
    new_x = float(new_x)
    new_y = float(new_y)
    ratio = float(ratio)
    gwid=gmpy.mpf(wid)

    greal = gmpy.mpf(real)
    gimag = gmpy.mpf(imag)

    greal += (gwid * new_x)
    gimag += (gwid * new_y)

    gwid *= ratio
    return redirect("/explore/%s/%s/%s/%s/" % (greal, gimag, gwid, iters))


@app.route("/explore/<real>/<imag>/<wid>/<iters>/pan/<direction>/")
def pan_view(real, imag, wid, iters, direction):
    greal = gmpy.mpf(real)
    gimag = gmpy.mpf(imag)
    gwid=gmpy.mpf(wid)
    if direction == "up":
        gimag -= gwid/2
    elif direction == "down":
        gimag += gwid/2
    elif direction == "left":
        greal -= gwid/2
    elif direction == "right":
        greal += gwid/2
    return redirect("/explore/%s/%s/%s/%s/" % (greal, gimag, gwid, iters))


@app.route("/explore/<real>/<imag>/<wid>/<iters>/img/")
def render(real, imag, wid, iters):
    out_wid = request.args.get("w", "200")
    out_hei = request.args.get("h", "200")
    mand = check_png("mandel", [real, imag, wid, iters, "/dev/stdout", out_wid, out_hei])
    colored = check_png("2bw", ["/dev/stdin", "/dev/stdout"], stdin=mand)
    return colored, 200, {"Content-type": "image/png"}


@app.route("/generate/<real>/<imag>/<wid>/<iters>/")
def generate(real="0", imag="0", wid="4", iters="50"):
    iters = int(float(iters))
    return render_template("generate.html", **locals())


def get_range(num):
    img = "../out2/reduced/%s.png" % num
    return subprocess.check_output(["./info", img])


#@app.route("/<int:num>/")
#def edit(num):
#    return render_template("map.html", num=int(num), map=get_map(), range=get_range(num))


@app.route("/image/<int:num>/")
def image(num):
    img = "../out2/reduced/%s.png" % num
    out = subprocess.check_output(["./pp", "map.json", img, "/dev/stdout"])
    return out, 200, {"Content-type": "image/png"}


@app.route("/<int:img>/del/<int:num>/")
def delete(img, num):
    jj = get_json()
    del jj[str(num)]
    set_json(jj)
    return redirect("/%s/" % img)


@app.route("/<int:img>/edit/<int:num>/<color>/")
def xxx(img, num, color):
    jj = get_json()
    jj[str(num)] = color
    set_json(jj)
    return redirect("/%s/" % img)


@app.route("/favicon.ico/")
def fav():
    return app.send_static_file("favicon.ico")

if __name__ == '__main__':
    app.run(debug=True)
