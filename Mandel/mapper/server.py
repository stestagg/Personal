#!/usr/bin/python

from flask import Flask, request, redirect, render_template 
import subprocess
import json

app = Flask(__name__)


SIZE=200

def get_map():
    return open("map.json", "r").read()


def get_json():
    return json.loads(get_map())


def set_json(val):
    with open("map.json", "w") as fp:
        return json.dump(val, fp)


@app.route("/")
def index():
    return redirect("/1157/")


def get_range(num):
    img = "../out2/reduced/%s.png" % num
    return subprocess.check_output(["./info", img])


@app.route("/<int:num>/")
def edit(num):
    return render_template("index.html", num=int(num), map=get_map(), range=get_range(num))


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
    return ""

if __name__ == '__main__':
    app.run(debug=True)
