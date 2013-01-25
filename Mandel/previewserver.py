from flask import Flask, request, redirect
import subprocess
import gmpy

gmpy.set_minprec(200)

app = Flask(__name__)


@app.route('/')
def hello_world():
    args = dict(x=0, y=0, wid=2, iters=10)
    for k, v in request.args.iteritems():
        args[k] = v
    args["i2"] = int(int(args["iters"]) * 1.2)
    return """
    <html>
    <body>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
    <script type="text/javascript">
    $(function(){
        $("#i").click(function(e){
            var ox = e.offsetX;
            var oy = e.offsetY;
            window.location = "/s/" + ox + "/" + oy + "" + window.location.search;
            })
        })
    </script>
    <dl>
        <dt>X</dt><dd>%(x)s</dd>
        <dt>Y</dt><dd>%(y)s</dd>
        <dt>Width</dt><dd>%(wid)s</dd>
        <dt>Iters</dt><dd>%(iters)s <a href="?iters=%(i2)s&x=%(x)s&y=%(y)s&wid=%(wid)s">%(i2)s</a></dd>
    </dl>
    <img src="/m?iters=%(iters)s&x=%(x)s&y=%(y)s&wid=%(wid)s" id="i"/>
    <a href="/out/0.98/?iters=%(iters)s&x=%(x)s&y=%(y)s&wid=%(wid)s">out</a>
    </body>
    </html>""" % args

SIZE=200

@app.route("/m")
def mandel():
    iters =  request.args.get("iters", "10")
    x = request.args.get("x", "0")
    y = request.args.get("y", "0")
    wid = request.args.get("wid", "2")
    out = subprocess.check_output(["./bin/mandel", x, y, wid, iters, "/dev/stdout", str(SIZE), str(SIZE)])
    return out, 200, {"Content-type": "image/png"}

@app.route("/s/<int:dx>/<int:dy>")
def subdivide(dx, dy):
    iters =  request.args.get("iters", "10")
    x = request.args.get("x", "0")
    y = request.args.get("y", "0")
    wid = request.args.get("wid", "2")
    gwid=gmpy.mpf(wid)
    dx = ((int(dx)/float(SIZE)) - 0.5) * (gwid*2)
    dy = ((int(dy)/float(SIZE)) - 0.5) * (gwid*2)
    gx = gmpy.mpf(x) + dx
    gy = gmpy.mpf(y) + dy
    gwid/=5
    return redirect("/?iters=%s&x=%s&y=%s&wid=%s" % (iters, str(gx), str(gy), str(gwid)))

@app.route("/out/<ratio>/")
def out(ratio):
    iters = int(request.args.get("iters", "10"))
    x = request.args.get("x", "0")
    y = request.args.get("y", "0")
    wid = request.args.get("wid", "2")

    target_wid = gmpy.mpf(wid)
    gx = gmpy.mpf(x)
    gy = gmpy.mpf(y)
    wid=gmpy.mpf(6)

    

    return "%s, %s, %s, %i" % (x, y, wid, iters)

if __name__ == '__main__':
    app.run(debug=True)
