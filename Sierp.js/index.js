    var sides = 7;
    var has_centre = false;
    var sierp = new Sierp(400, 400, 1);

    function onclick(){
        startLoad();
        window.setTimeout("run(1)", 10);
    }

    function auto(){
        startLoad();
        window.setTimeout("autorun()", 10);
    }

    function autorun(){
        var did_stuff = sierp.safe_next();
        sierp.render($("#sierp")[0]);
        if (did_stuff){
            window.setTimeout("autorun()", 100);
        }else{
            stopLoad();
        }   
    }

    function update_points(){
        sierp.set_points(sides);
        if (has_centre){
            sierp.points.push(new Point(200, 200));
        }
        $("#sides").text(sides);
        sierp.drawPoints($("#sierp")[0]);
    }

    function more(){
        sides += 1;
        update_points();
    }
    function less(){
        sides -= 1;
        if (sides < 2) sides=2;
        update_points();
    }

    function centre(){
        has_centre = $(this).is(':checked');
        update_points();
    }

    function startLoad(){
        $("#loading").show();
        $("button", ".main").attr("disabled", "disabled");
    }

    function stopLoad(){
        $("#loading").fadeOut("fast");
        $("button", ".main").attr("disabled", false);
    }

    function run(num){
        for (var i=0; i<num; ++i){
            sierp.next();
        }
        sierp.render($("#sierp")[0]);
        $("button").removeAttr("disabled");
        $("#loading").fadeOut("fast");
    }

    function result_click(){
        $(".result").removeClass("active");
        $(this).addClass("active");
        var new_img = new Image();
        new_img.src = $("img", this)[0].src;
        var canvas = $("#sierp")[0];
        var ctx = canvas.getContext("2d");
        ctx.clearRect(0,0,canvas.width,canvas.height);
        ctx.drawImage(new_img, 0, 0, canvas.width, canvas.height);
    };

    function add_icon(data, name){
        $("button", ".results").attr("disabled", false);
        var result = $('<div class="result"><img src="" class="result_img"/>'
                      +'<span class="sel"><span class="label"></span></span></div>');
        $("img", result).attr("src", data); 
        $("span.label", result).text(name);
        result.click(result_click);
        $(".icon_list .panel_in").append(result);
        $(".icon_list .panel_in").scrollTop(result.position().top);
    };

    function reset_icons(){
        $(".icon_list .panel_in").html("");
        $(this).attr("disabled", "disabled");  
    };

    function save_result(){
        var result_data = $("#sierp")[0].toDataURL();
        add_icon(result_data, "Result");
    };

    $(function(){
        sierp.set_points(sides);
        window.setTimeout("update_points()", 10);
        stopLoad();
        $("#next").click(onclick);
        $("#reset_icons").click(reset_icons);
        $("#save").click(save_result);
        $("#less").click(less);
        $("#auto").click(auto);
        $("#centre").click(centre);
        $("#more").click(more);
    });
